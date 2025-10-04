#pragma once

#include <algorithm>
#include <atomic>
#include <functional>
#include <map>
#include <mutex>
#include <optional>
#include <thread>
#include <tuple>
#include <vector>
#include <queue>
#include <limits>

template <typename T>
bool comp(const T &a, const T &b) { 
    return a.first < b.first; 
}

static inline size_t clampThreads(size_t requestedThreads) {
    unsigned int hwThreads = std::thread::hardware_concurrency();

    if (hwThreads == 0) hwThreads = 1;
    if (requestedThreads == 0) return hwThreads;

    return std::min<size_t>(requestedThreads, hwThreads);
}

template <typename T>
void inplaceDeduplicated(std::vector<T>& vec) {
    size_t writeIndex = 0;

    for (size_t readIndex = 0; readIndex < vec.size(); ++readIndex) {
        if (writeIndex > 0 && vec[writeIndex - 1].first == vec[readIndex].first) {
            vec[writeIndex - 1] = vec[readIndex];
        } else {
            vec[writeIndex++] = vec[readIndex];
        }
    }

    vec.resize(writeIndex);
}

static void mergeRanges(
    const std::vector<std::pair<int,int>>& leftVec, size_t leftLow, size_t leftHigh,
    const std::vector<std::pair<int,int>>& rightVec, size_t rightLow, size_t rightHigh,
    std::vector<std::pair<int,int>>& outVec, size_t outLow
) {
    size_t i = leftLow, j = rightLow, k = outLow;
    while (i < leftHigh && j < rightHigh) {
        if (leftVec[i].first < rightVec[j].first) {
            outVec[k++] = leftVec[i++];
        } else if (leftVec[i].first > rightVec[j].first) {
            outVec[k++] = rightVec[j++];
        } else {
            outVec[k++] = rightVec[j++];
            ++i;
        }
    }
    while (i < leftHigh) outVec[k++] = leftVec[i++];
    while (j < rightHigh) outVec[k++] = rightVec[j++];
}

class MultiThreadedBST {
public:
    explicit MultiThreadedBST(size_t threadCount = std::thread::hardware_concurrency())
        : threadCount_(clampThreads(threadCount))
    {}

    void insert(const std::vector<std::pair<int,int>>& items) {
        if (items.empty()) return;

        std::vector<std::pair<int,int>> batch = items;

        parallelSortPairsByKey(batch);
        inplaceDeduplicated(batch);

        std::vector<std::pair<int,int>> existingCopy;
        {
            std::lock_guard<std::mutex> lock(dataMutex_);
            existingCopy = sortedData_;
        }

        if (existingCopy.empty()) {
            std::lock_guard<std::mutex> lock(dataMutex_);
            sortedData_.swap(batch);
            return;
        }

        if (batch.empty()) return;

        std::vector<std::pair<int, int>> mergedResult;
        mergedResult.resize(existingCopy.size() + batch.size());

        size_t threadsToUse = threadCount_;
        mergeRanges(existingCopy, 0, existingCopy.size(),
                            batch, 0, batch.size(),
                            mergedResult, 0);

        inplaceDeduplicated(mergedResult);

        {
            std::lock_guard<std::mutex> lk(dataMutex_);
            sortedData_.swap(mergedResult);
        }
    }

    void search(const std::vector<int>& keys, std::vector<std::optional<int>>& results) const {
        results.resize(keys.size());
        if (keys.empty()) return;
        
        const std::vector<std::pair<int,int>> *dataPtr = nullptr;
        {
            std::lock_guard<std::mutex> lk(dataMutex_);
            dataPtr = &sortedData_;
        }
        const auto& dataRef = *dataPtr;

        const size_t nthreads = std::min<size_t>(threadCount_, keys.size());
        std::vector<std::thread> workerThreads;
        workerThreads.reserve(nthreads);

        size_t baseChunk = keys.size() / nthreads;
        size_t remainder = keys.size() % nthreads;

        auto worker = [&](size_t startIndex, size_t endIndex){
            for (size_t k = startIndex; k < endIndex; ++k) {
                int key = keys[k];
                size_t low = 0, high = dataRef.size();

                std::optional<int> found = std::nullopt;

                while (low < high) {
                    size_t mid = low + (high - low) / 2;
                    if (dataRef[mid].first == key) {
                        found = dataRef[mid].second;
                        break;
                    } else if (dataRef[mid].first < key) {
                        low = mid + 1;
                    } else {
                        high = mid;
                    }
                }
                results[k] = found;
            }
        };

        size_t start = 0;

        for (size_t t = 0; t < nthreads; ++t) {
            size_t chunkSize = baseChunk + (t < remainder ? 1 : 0);
            size_t end = start + chunkSize;

            workerThreads.emplace_back(worker, start, end);

            start = end;
        }

        for (auto &th : workerThreads) th.join();
    }

    size_t size() const {
        std::lock_guard<std::mutex> lock(dataMutex_);
        return sortedData_.size();
    }

    size_t threads() const { return threadCount_; }

private:
    void parallelSortPairsByKey(std::vector<std::pair<int,int>>& vec) const {
        if (vec.size() < 1024 || threadCount_ <= 1) {
            std::sort(vec.begin(), vec.end(), comp<std::pair<int, int>>);
            return;
        }

        const size_t T = std::min<size_t>(threadCount_, vec.size());

        std::vector<std::pair<size_t,size_t>> ranges;
        ranges.reserve(T);

        size_t chunk = vec.size() / T;
        size_t rem = vec.size() % T;

        size_t s = 0;

        for (size_t i = 0; i < T; ++i) {
            size_t c = chunk + (i < rem ? 1 : 0);
            ranges.emplace_back(s, s + c);

            s += c;
        }

        std::vector<std::thread> threads;
        threads.reserve(T);

        for (size_t i = 0; i < T; ++i) {
            auto st = ranges[i].first;
            auto ed = ranges[i].second;

            threads.emplace_back([st, ed,&vec](){
                std::sort(vec.begin()+st, vec.begin()+ed, comp<std::pair<int, int>>); 
            });
        }

        for (auto &t : threads) t.join();

        std::priority_queue<std::tuple<int,int,size_t,size_t>,
                            std::vector<std::tuple<int,int,size_t,size_t>>,
                            std::greater<>> minHeap;

        std::vector<size_t> chunkStart(T), chunkEnd(T);

        for (size_t i = 0; i < T; ++i) {
            chunkStart[i] = ranges[i].first;
            chunkEnd[i] = ranges[i].second;

            if (chunkStart[i] < chunkEnd[i]) {
                const auto &p = vec[chunkStart[i]];
                minHeap.emplace(p.first, p.second, i, chunkStart[i]);
            }
        }

        std::vector<std::pair<int,int>> outVec;
        outVec.reserve(vec.size());

        while (!minHeap.empty()) {
            auto tup = minHeap.top(); minHeap.pop();

            int key = std::get<0>(tup);
            int value = std::get<1>(tup);

            size_t chunkIndex = std::get<2>(tup);
            size_t pos = std::get<3>(tup);

            outVec.emplace_back(key, value);

            size_t nextPos = pos + 1;

            if (nextPos < chunkEnd[chunkIndex]) {
                const auto &np = vec[nextPos];
                minHeap.emplace(np.first, np.second, chunkIndex, nextPos);
            }
        }

        vec.swap(outVec);
    }

private:
    size_t threadCount_;

    mutable std::mutex dataMutex_;
    std::vector<std::pair<int,int>> sortedData_; 
};

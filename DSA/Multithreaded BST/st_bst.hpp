#pragma once

#include <algorithm>
#include <functional>
#include <map>
#include <optional>
#include <vector>

class SingleThreadedBST {
public:
    SingleThreadedBST() = default;

    void insert(const std::vector<std::pair<int,int>>& items) {
        for (auto &kv : items) {
            treeMap_[kv.first] = kv.second;
        }
    }

    void search(const std::vector<int>& keys, std::vector<std::optional<int>>& results) const {
        results.resize(keys.size());
        for (size_t i = 0; i < keys.size(); ++i) {
            auto it = treeMap_.find(keys[i]);

            if (it == treeMap_.end()) 
                results[i] = std::nullopt;
            else 
                results[i] = it->second;
        }
    }

    size_t size() const { return treeMap_.size(); }

private:
    std::map<int,int> treeMap_;
};
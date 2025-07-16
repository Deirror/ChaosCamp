#include <fstream>
#include <chrono>
#include <string>

class Timer {
public:
    Timer(const std::string& filepath, const std::string& profileText) 
        : filepath_(filepath), profileText_(profileText), start_(std::chrono::steady_clock::now()) {}
    ~Timer();

private:
    std::string filepath_;
    std::string profileText_;

    std::chrono::time_point<std::chrono::steady_clock> start_;
};


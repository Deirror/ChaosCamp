#include "Timer.h"

Timer::~Timer() {
	auto end = std::chrono::steady_clock::now();
	auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_).count();
	std::ofstream file(filepath_, std::ios::app);
	if (file.is_open()) {
		file << profileText_ << ": " << duration_ms << " ms\n";
	}
}

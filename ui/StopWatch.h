#pragma once
#include <chrono>

class StopWatch
{
public:
	StopWatch() {
		_start = std::chrono::steady_clock::now();
	}

	void reset() {
		_start = std::chrono::steady_clock::now();
	}

	double elapsed_s() {
		return std::chrono::duration<double>(std::chrono::steady_clock::now() - _start).count();
	}

	double elapsed_ms() {
		return std::chrono::duration<double, std::milli>(std::chrono::steady_clock::now() - _start).count();
	}

	double elapsed_us() {
		return std::chrono::duration<double, std::micro>(std::chrono::steady_clock::now() - _start).count();
	}

	double elapsed_ns() {
		return std::chrono::duration<double, std::nano>(std::chrono::steady_clock::now() - _start).count();
	}

private:
	std::chrono::steady_clock::time_point _start;
};


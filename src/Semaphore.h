#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
	Semaphore(int = 0);
	void up();
	void down();
private:
	std::mutex mx;
	std::condition_variable cnd;
	int count;
};

#include "ExitSemaphore.h"

ExitSemaphore::ExitSemaphore() : count{ 0 } {}

void ExitSemaphore::up() {
	std::lock_guard<std::mutex> lock(mx);
	count++;
}


void ExitSemaphore::down() {
	std::unique_lock<std::mutex> lock(mx);
	count--;
	cnd.notify_one();
}


void ExitSemaphore::wait() {
	std::unique_lock<std::mutex> lock(mx);
	auto pred = [&count = count]() { return count == 0; };
	cnd.wait(lock, pred);
}
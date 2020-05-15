#include "Semaphore.h"

Semaphore::Semaphore(int count) : count{ count } {}

void Semaphore::up() {
	std::lock_guard<std::mutex> lock(mx);
	count++;
	cnd.notify_one();
}

void Semaphore::down() {
	std::unique_lock<std::mutex> lock(mx);
	auto pred = [&count = count]() { return count > 0; };
	cnd.wait(lock, pred);
	count--;
}
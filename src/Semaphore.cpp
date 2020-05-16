#include "Semaphore.h"

Semaphore::Semaphore(int count) : count{ count } {}

void Semaphore::up() {
	std::lock_guard<std::mutex> lock(mx);
	count++;
	cnd.notify_one();
}

void Semaphore::down() {
	std::unique_lock<std::mutex> lock(mx);
	auto pred = [&count=count, &destroyed=destroyed]() { return count > 0 || destroyed; };
	cnd.wait(lock, pred);
	if (!destroyed) count--;
}


void Semaphore::destroy() {
	std::unique_lock<std::mutex> lock(mx);
	destroyed = true;
	cnd.notify_all();
}


bool Semaphore::isDestroyed() {
	return destroyed;
}

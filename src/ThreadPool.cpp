#include "ThreadPool.h"

ThreadPool::ThreadPool(int size) {
	for (int i = 0; i < size; i++) {
		std::thread thread{ &ThreadPool::runTasks, this };
		thread.detach();
	}
}

ThreadPool::~ThreadPool() {
	exitSema.wait();
}

void ThreadPool::enqueue(TASK task) {
	std::unique_lock<std::mutex> lock(queueMut);
	queue.emplace(task);
	lock.unlock();
	queueSema.up();
	exitSema.up();
}

void ThreadPool::runTasks() {
	queueSema.down();

	std::unique_lock<std::mutex> lock(queueMut);
	TASK task = queue.front();
	queue.pop();
	lock.unlock();

	task();
	exitSema.down();
	runTasks();
}
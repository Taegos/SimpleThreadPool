#include "ThreadPool.h"


ThreadPool::ThreadPool(int size) {
	for (int i = 0; i < size; i++) {
		std::thread thread{ &ThreadPool::runTasks, this };
		thread.detach();
		exitSema.up();
	}
}


ThreadPool::~ThreadPool() {
	queueSema.destroy(); //releases threads that are waiting for task
	exitSema.wait(); //waits for threads to finnish
}


void ThreadPool::enqueue(TASK task) {

	{
		std::lock_guard<std::mutex> lock(queueMut);
		queue.emplace(task);
	}

	queueSema.up();
}


void ThreadPool::runTasks() {

	queueSema.down();

	if (queueSema.isDestroyed() && queue.empty()) {
		exitSema.down();
		return;
	}

	TASK task;
	{
		std::lock_guard<std::mutex> lock(queueMut);
		task = queue.front();
		queue.pop();
	}
	
	task();
	runTasks();
}
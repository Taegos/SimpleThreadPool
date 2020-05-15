#pragma once

#include "Semaphore.h"
#include "ExitSemaphore.h"
#include <functional>
#include <queue>

class ThreadPool {

	using TASK = std::function<void()>;

public:
	ThreadPool(int);
	~ThreadPool();
	void enqueue(TASK);

private:
	ExitSemaphore exitSema;
	Semaphore queueSema;
	std::mutex queueMut;
	std::queue<TASK> queue;

	void runTasks();
};
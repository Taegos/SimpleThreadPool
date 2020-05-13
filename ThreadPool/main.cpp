// ThreadPool.cpp : Defines the entry point for the application.
//

#include "main.h"
#include <thread>
#include <functional>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>

class Semaphore {
public:
	Semaphore(int count = 0) : count{ count } {}

	void up() {
		std::lock_guard<std::mutex> lock(mx);
		count++;
		cnd.notify_one();
	}
	
	void down() {
		std::unique_lock<std::mutex> lock(mx);
		auto pred = [&count = count]() { return count > 0; };
		cnd.wait(lock, pred);
		count--;
	}

	void wait() {
		std::unique_lock<std::mutex> lock(mx);
		auto pred = [&count = count]() { return count == 0; };
		cnd.wait(lock, pred);
	}
	
private:
	std::mutex mx;
	std::condition_variable cnd;
	int count;
};


class ExitSemaphore {
public:
	ExitSemaphore() : count{ 0 } {}

	void up() {
		std::lock_guard<std::mutex> lock(mx);
		count++;
	}

	void down() {
		std::unique_lock<std::mutex> lock(mx);
		count--;
		cnd.notify_one();
	}

	void wait() {
		std::unique_lock<std::mutex> lock(mx);
		auto pred = [&count = count]() { return count == 0; };
		cnd.wait(lock, pred);
	}

private:
	std::mutex mx;
	std::condition_variable cnd;
	int count;
};


using TASK = std::function<void()>;

void semaTest() {
	using namespace std;

	Semaphore s;
	auto a = [&s]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		cout << "A UP" << endl;
		s.up();
	};

	auto b = [&s]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		cout << "B DOWN" << endl;
		s.down();
		cout << "B END" << endl;
	};

	auto c = [&s]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		cout << "C UP" << endl;
		s.up();
	};

	thread t0{ a };
	thread t1{ b };
	thread t2{ c };

	t0.detach();
	t1.detach();
	t2.detach();

	std::this_thread::sleep_for(std::chrono::milliseconds(1200));
	cout << "MAIN DOWN" << endl;
	s.down();
	cout << "MAIN END" << endl;

	//B DOWN
	//MAIN DOWN
	//A UP
	//B END
	//C UP
	//MAIN END
}

class ThreadPool {

public:
	ThreadPool(int size) {
		for (int i = 0; i < size; i++) {
			std::thread thread {&ThreadPool::runTasks, this };
			thread.detach();
		}
	}

	~ThreadPool() {
		exitSema.wait();
	}

	void enqueue(TASK task) {
		std::unique_lock<std::mutex> lock(queueMut);
		queue.emplace(task);		
		lock.unlock();
		queueSema.up();
		exitSema.up();
	}


private:
	ExitSemaphore exitSema;
	Semaphore queueSema;
	std::mutex queueMut;
	std::queue<TASK> queue;

	void runTasks() {
		queueSema.down();	
		
		std::unique_lock<std::mutex> lock(queueMut);
		TASK task = queue.front();
		queue.pop();
		lock.unlock();

		task();
		exitSema.down();
		runTasks();
	}
};


TASK build(int ms) {
	auto fn = [ms]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		std::cout << std::this_thread::get_id() << " done" << std::endl;
	};
	return fn;
}


int main()
{
	ThreadPool pool{ 100 };
	pool.enqueue(build(1500));
	pool.enqueue(build(1600));
	pool.enqueue(build(1900));
	pool.enqueue(build(1700));
	pool.enqueue(build(1650));

	pool.enqueue(build(2100));
	pool.enqueue(build(1350));
	pool.enqueue(build(1762));
	
	return 0;
}
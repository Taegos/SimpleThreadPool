#include "ThreadPool.h"
#include "ExitSemaphore.h"
#include "Semaphore.h"
#include <iostream>
#include <functional>

using namespace std;

void semaphoreDemo() {

	Semaphore s;

	auto a = [&s]() {
		this_thread::sleep_for(chrono::milliseconds(1500));
		cout << "A UP" << endl;
		s.up();
	};

	auto b = [&s]() {
		this_thread::sleep_for(chrono::milliseconds(1000));
		cout << "B DOWN" << endl;
		s.down();
		cout << "B END" << endl;
	};

	auto c = [&s]() {
		this_thread::sleep_for(chrono::milliseconds(2000));
		cout << "C UP" << endl;
		s.up();
	};

	thread t0{ a };
	thread t1{ b };
	thread t2{ c };

	t0.detach();
	t1.detach();
	t2.detach();

	this_thread::sleep_for(chrono::milliseconds(1200));
	cout << "MAIN DOWN" << endl;
	s.down();
	cout << "MAIN END" << endl;
}


function<void()> timer(int ms) {
	auto fn = [ms]() {
		this_thread::sleep_for(chrono::milliseconds(ms));
		cout << "Thread " << this_thread::get_id() << " done" << std::endl;
	};
	return fn;
}


void threadPoolDemo() {

	ThreadPool pool{ 5 };

	pool.enqueue(timer(1500));
	pool.enqueue(timer(1600));
	pool.enqueue(timer(1900));
	pool.enqueue(timer(1700));
	pool.enqueue(timer(1650));

	pool.enqueue(timer(2100));
	pool.enqueue(timer(1350));
	pool.enqueue(timer(1762));
}


int main()
{
	cout << "Semaphore Demo:" << endl << endl;
	this_thread::sleep_for(chrono::milliseconds(600)); 
	semaphoreDemo();
	this_thread::sleep_for(chrono::milliseconds(600));

	cout << endl << "Threadpool Demo:" << endl << endl;
	this_thread::sleep_for(chrono::milliseconds(600));
	threadPoolDemo();
	return 0;
}
#include "ThreadPool.h"
#include "ExitSemaphore.h"
#include "Semaphore.h"
#include <functional>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

using namespace std;

void semaphoreTest() {

	Semaphore s;
	
	vector<string> expected {
		"B DOWN", "MAIN DOWN", "A UP", "B END", "C UP", "MAIN END"
	};
	vector<string> actual{};

	auto a = [&]() {
		this_thread::sleep_for(chrono::milliseconds(1500));
		actual.push_back("A UP");
		s.up();
	};

	auto b = [&]() {
		this_thread::sleep_for(chrono::milliseconds(1000));
		actual.push_back("B DOWN");
		s.down();
		actual.push_back("B END");
	};

	auto c = [&]() {
		this_thread::sleep_for(chrono::milliseconds(2000));
		actual.push_back("C UP");
		s.up();
	};

	thread t0{ a };
	thread t1{ b };
	thread t2{ c };

	t0.detach();
	t1.detach();
	t2.detach();

	this_thread::sleep_for(chrono::milliseconds(1200));

	actual.push_back("MAIN DOWN");
	s.down();
	actual.push_back("MAIN END");


	assert(("Excpected size must be equal actual size", expected.size() == actual.size()));

	for (int i = 0; i < expected.size(); i++) {
		assert(("Values must be the same", expected[i] == actual[i]));
	}

	cout << "Semaphore test passed" << endl;
}


int main()
{
	semaphoreTest();
	return 0;
}
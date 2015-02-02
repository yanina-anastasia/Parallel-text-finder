#include "ThreadPool.h"


void ThreadPool:: appendFn(fn_type fn) {
	boost::unique_lock<boost::mutex> locker(mutex);
	fqueue.push(fn);
	cond.notify_one();
}	
	
size_t ThreadPool:: getTaskCount() { 
	boost::unique_lock<boost::mutex> locker(mutex);
	return fqueue.size();		
}

bool ThreadPool:: isEmpty() {
	boost::unique_lock<boost::mutex> locker(mutex);
	return fqueue.empty();
}

void ThreadPool::threadFn() {
	while (true) {
		boost::unique_lock<boost::mutex> locker(mutex);
		cond.wait(locker, [&](){ return !fqueue.empty() || !enabled; });	
		if (!enabled) {
			break;
		}
		if (!fqueue.empty()) {
			fn_type fn = fqueue.front();
			fqueue.pop();
			locker.unlock();
			fn();
			locker.lock();		
		}
	}
}

/*ThreadPool:: ThreadPool(): enabled(true), fqueue() {
	threadNumber = std::thread::hardware_concurrency();
	for (size_t i = 0; i < threadNumber; i++) {
		threads.push_back(std::thread(&ThreadPool::threadFn, this));
	}
}*/

ThreadPool:: ThreadPool(size_t thread): enabled(true), fqueue() {
	if (thread == 0) {
		thread = 1;
	}
	for (size_t i = 0; i < thread; i++) {
		threads.push_back(std::thread(&ThreadPool::threadFn, this));
	}
	threadNumber = thread;
}

ThreadPool:: ThreadPool(ThreadPool & threadPool) {}

ThreadPool:: ~ThreadPool() {
	{
		boost::unique_lock<boost::mutex> locker(mutex);
		enabled = false;
		cond.notify_all();
	}
	for (int i = 0; i < threadNumber; i++) {
		threads[i].join();
	}
}


void factorial (int n) {
	int result = 1;
	for (int i = 2; i <= n; i++) {
		result *= i;
	}
	std::cout << "Factorial done: " << result << std::endl;
}

void printer (std::string print) {
	std:: cout << print << std::endl;
}

void square (int n) {
	std:: cout << "n * n done: " << n * n << std::endl;
}

void sort (std::vector<int> n) {
	for (int i = 0; i < n.size() - 1; i++) {
		for (int j = 0; j < n.size() - i - 1; j++) {
			if (n[j] > n[j + 1]) {
				int c = n[j];
				n[j] = n[j + 1];
				n[j + 1] = c;
			}
		}
	}
	std::cout << "Sort done" << std::endl;
}

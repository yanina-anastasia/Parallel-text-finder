#include <thread>
#include <queue>
#include <mutex>
#include <memory>
#include <vector>
#include <string>
#include <boost\thread\mutex.hpp>
#include <boost\thread\condition_variable.hpp>

typedef std::function<void()> fn_type;
 
class ThreadPool {
public:
	ThreadPool(size_t threads);
	ThreadPool(ThreadPool & threadPool);

	~ThreadPool();
	
	void appendFn(fn_type fn);
	size_t getTaskCount();
	bool isEmpty();
	void threadFn();

private:
	int threadNumber;
	boost::condition_variable cond;
	std::queue<fn_type> fqueue;		
	boost::mutex mutex;		
	std::vector<std::thread> threads;
	bool enabled; 
};

void factorial(int n);
void printer(std::string s);
void square(int n);
void sort(std::vector<int> n);

// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <vector>
#include <mutex>
#include "math.h"

using namespace std;

static const int MAX = 10e8; // ①
static double sum = 0; // ②

static std::mutex exclusive;

void worker(int min, int max) 
{
	for (int i = min; i <= max; i++) 
	{
		sum += sqrt(i);
	}
}


void worker_protected(int min, int max)
{
	double temp = 0;
	for (int i = min; i <= max; i++)
	{
		temp += sqrt(i);
	}
	exclusive.lock();
	sum += temp;
	exclusive.unlock();
}

void single() 
{
	auto start_time = chrono::steady_clock::now();
	sum = 0;
	//一个线程处理
	worker(0, MAX);
	auto end_time = chrono::steady_clock::now();
	auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
	cout << "Serail task finish, " << ms << " ms consumed, Result: " << sum << endl;
}

void multi_thread_no_protected() 
{
	auto start_time = chrono::steady_clock::now();
	sum = 0;

	//多个线程 
	unsigned concurrent_count = thread::hardware_concurrency(); // ①
	cout << "hardware_concurrency: " << concurrent_count << endl;
	vector<thread> threads;
	int min = 0;
	sum = 0;
	for (int i = 0; i < concurrent_count; i++)
	{
		int max = MAX / concurrent_count * (i + 1);
		threads.push_back(thread(worker, min, max));
		min = max + 1;
	}
	for (thread& thread : threads)
	{
		thread.join();
	}
	auto end_time = chrono::steady_clock::now();
	auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
	cout << "Serail task finish, " << ms << " ms consumed, Result: " << sum << endl;

}

void multi_thread_protected()
{
	auto start_time = chrono::steady_clock::now();
	sum = 0;

	//多个线程 
	unsigned concurrent_count = thread::hardware_concurrency(); // ①
	cout << "hardware_concurrency: " << concurrent_count << endl;
	vector<thread> threads;
	int min = 0;
	sum = 0;
	for (int i = 0; i < concurrent_count; i++)
	{
		int max = MAX / concurrent_count * (i + 1);
		threads.push_back(thread(worker_protected, min, max));
		min = max + 1;
	}
	for (thread& thread : threads)
	{
		thread.join();
	}
	auto end_time = chrono::steady_clock::now();
	auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
	cout << "Serail task finish, " << ms << " ms consumed, Result: " << sum << endl;

}

 int main()
 {
	//single();
	//multi_thread_no_protected();
	multi_thread_protected();


 	return 0;
 }

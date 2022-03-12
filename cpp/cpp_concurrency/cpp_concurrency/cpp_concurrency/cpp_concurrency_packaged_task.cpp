// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <vector>
#include <future>
#include <mutex>
#include "math.h"

using namespace std;

static const int MAX = 10e8; // ①
static double sum = 0; // ②

static std::mutex exclusive;

double worker(int min, int max)
{
	double temp = 0;
	for (int i = min; i <= max; i++)
	{
		temp += sqrt(i);
	}
	return temp;
}


void multi_thread_protected()
{
	auto start_time = chrono::steady_clock::now();
	sum = 0;

	//多个线程 
	unsigned concurrent_count = thread::hardware_concurrency(); // ①
	cout << "hardware_concurrency: " << concurrent_count << endl;
	vector<future<double>> futures;
	int min = 0;
	sum = 0;
	for (int i = 0; i < concurrent_count; i++)
	{
		int max = MAX / concurrent_count * (i + 1);
		packaged_task<double(int, int)> task(worker);
		futures.push_back(task.get_future());
		thread t(std::move(task), min, max);
		t.detach();

		min = max + 1;
	}
	for (auto& f : futures)
	{
		//这里的get会等待线程执行完成
		sum += f.get();
	}
	auto end_time = chrono::steady_clock::now();
	auto ms = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
	cout << "Serail task finish, " << ms << " ms consumed, Result: " << sum << endl;

}

 int main()
 {
	multi_thread_protected();

 	return 0;
 }

// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <future>

using namespace std;


static const int MAX = 10e8;
static double sum = 0;

void worker(int min, int max) {
	for (int i = min; i <= max; i++) {
		sum += sqrt(i);
	}
}

class Worker {

public:
	Worker(int min, int max):mMin(min),mMax(max),mResult(0)
	{

	}

public:
	void work() {
		for (int i = mMin; i <= mMax; i++) {
			mResult += sqrt(i);
		}
	}

	double getResult() {
		return mResult;
	}

private:
	int mMin;
	int mMax;
	double mResult;
};

static void asyncWork() {

	//是否新建线程还是当前线程执行时靠编译器
   sum = 0;
   auto f = async(worker, 0, MAX);
   f.wait();
   cout << "Async task finish, result: " << sum << endl << endl;
}

static void asyncLambda() {
	sum = 0;
	cout << " main thread id: " << this_thread::get_id() << endl << endl;
	auto f = async([](int min, int max) {
		cout << "Async task thread id: " << this_thread::get_id() << endl << endl;
		for (int i = min; i <= max; i++) {
			sum += sqrt(i);
		}
	}, 0, MAX);
	f.wait();
	cout << "Async task finish, result: " << sum << endl << endl;
}

 int main()
 {
	 asyncWork();
	 asyncLambda();

	 Worker w(0, MAX);
	 auto f = async(&Worker::work, &w);
	 f.wait();
	 cout << "Async task finish, result: " << w.getResult() << endl << endl;

 	return 0;
 }

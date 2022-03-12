// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <mutex>

using namespace std;


void init() 
{
	cout << "Hello init "  << endl;
}

void worker(once_flag* flag)
{
    call_once(*flag, init);
	cout << "worker "  << endl;
}

 int main()
 {
   once_flag flag;
 	thread thread1(worker, &flag);
   thread thread2(worker, &flag);
 	thread thread3(worker, &flag);
 	thread1.join();
 	thread2.join();
 	thread3.join();

 	return 0;
 }

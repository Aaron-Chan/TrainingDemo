// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>

using namespace std;


void func() 
{
	cout << "Hello thread1 "  << endl;
}

void funcWithParams(int i)
{
	cout << "Hello thread2 " << i << endl;
}

 int main()
 {
 	//基础
 	thread thread1(func);
 	//带参数
 	thread thread2(funcWithParams, 2);
 	//lambda
 	thread thread3([]() {
 		cout << "Hello thread3" << endl;
 	});
 	thread1.join();
 	thread2.join();
 	thread3.join();

 	return 0;
 }

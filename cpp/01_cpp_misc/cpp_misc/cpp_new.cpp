// cpp_concurrency.cpp: 定义应用程序的入口点。
//


#include <thread>
#include <iostream>

using namespace std;


class A {

public:
    A(int i):i(i) {}
public:
    void print() 
    {
        cout << "A:i:" << i << endl;
    }
    int i;

};

int main()
{
    const int n = 2;
    char memory_array[n * sizeof(A)];
    for (int i = 0; i < n; i++)
    {
        new (memory_array + i * sizeof(A))A(i);
    }
    A* a_array = (A*)(memory_array);
    for (int i = 0; i < n; i++)
    {
        a_array[i].print();
        a_array[i].~A();
    }

    

 }

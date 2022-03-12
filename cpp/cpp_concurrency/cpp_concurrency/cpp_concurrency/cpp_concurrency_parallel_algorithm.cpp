// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <vector>
#include <random>
#include <algorithm>
#include <execution>

using namespace std;


void generateRandomData(vector<double>& collection, int size) {
	random_device rd;
	mt19937 mt(rd());
	uniform_real_distribution<double> dist(1.0, 100.0);
	for (int i = 0; i < size; i++) {
		collection.push_back(dist(mt));
	}
}


 int main()
 {
	 vector<double> collection;
	 generateRandomData(collection, 10e6); // ①

	 vector<double> copy1(collection); // ②
	 vector<double> copy2(collection);
	 vector<double> copy3(collection);

	 auto time1 = chrono::steady_clock::now(); // ③
	 std::sort(std::execution::seq, copy1.begin(), copy1.end());
	 auto time2 = chrono::steady_clock::now();
	 auto duration = chrono::duration_cast<chrono::milliseconds>(time2 - time1).count();
	 cout << "Sequenced sort consuming " << duration << "ms." << endl; // ⑤

	 time1 = chrono::steady_clock::now(); // ③
	 std::sort(std::execution::par, std::begin(copy2), std::end(copy2));
	 time2 = chrono::steady_clock::now(); // ③
	 duration = chrono::duration_cast<chrono::milliseconds>(time2 - time1).count();
	 cout << "par sort consuming " << duration << "ms." << endl; // ⑤

	 time1 = chrono::steady_clock::now(); // ③
	 std::sort(std::execution::par_unseq, std::begin(copy3), std::end(copy3));
	 time2 = chrono::steady_clock::now(); // ③
	 duration = chrono::duration_cast<chrono::milliseconds>(time2 - time1).count();
	 cout << "par_unseq sort consuming " << duration << "ms." << endl; // ⑤



 	return 0;
 }

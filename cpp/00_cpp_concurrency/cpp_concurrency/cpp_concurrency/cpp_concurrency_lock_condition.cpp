// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <set>
#include <condition_variable>
#include "math.h"




using namespace std;

static mutex print_mutex;



class Account {
public:
	Account(string name, double money) : mName(name), mMoney(money) {};

public:
	void changeMoney(double amount) {
		std::unique_lock<std::mutex> lock(mMoneyLock);
		mConditionVar.wait(lock, [amount, this ] {
			return (mMoney + amount) >= 0;
		});

		mMoney += amount;

		mConditionVar.notify_all();
	}
	string getName() {
		return mName;
	}
	double getMoney() {
		return mMoney;
	}
	mutex* getLock() {
		return &mMoneyLock;
	}

private:
	string mName;
	double mMoney;
	mutex mMoneyLock;
	std::condition_variable mConditionVar;
};


class Bank {
public:
	void addAccount(Account* account) {
		mAccounts.insert(account);
	}


	bool transferMoney(Account* accountA, Account* accountB, double amount) {

		accountA->changeMoney(-amount); // ③
		accountB->changeMoney(amount);
		return true;
	}

	double totalMoney() const {
		double sum = 0;
		for (auto a : mAccounts) {
			sum += a->getMoney();
		}
		return sum;
	}

private:
	set<Account*> mAccounts;
};

void randomTransfer(Bank* bank, Account* accountA, Account* accountB) {

	while (true)
	{
		double randomMoney = rand() % 100 +1;//1-100
		bank->transferMoney(accountA, accountB, randomMoney);
		print_mutex.lock();
		cout << "Transfer " << randomMoney << " from " << accountA->getName()
			<< " to " << accountB->getName()
			<< ", Bank totalMoney: " << bank->totalMoney() << endl;
		print_mutex.unlock();
	}
}

 int main()
 {
	 Account a("a", 100);
	 Account b("b", 100);
	 Bank bank;
	 bank.addAccount(&a);
	 bank.addAccount(&b);

	 
	 thread thread_a(randomTransfer, &bank, &a, &b);
	 thread thread_b(randomTransfer, &bank, &b, &a);

	 thread_a.join();
	 thread_b.join();

 	return 0;
 }

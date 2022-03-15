// cpp_concurrency.cpp: 定义应用程序的入口点。
//

#include "cpp_concurrency.h"

#include <thread>
#include <vector>
#include <string>
#include <mutex>
#include <set>
#include "math.h"

using namespace std;

static mutex print_mutex;



class Account {
public:
	Account(string name, double money) : mName(name), mMoney(money) {};

public:
	void changeMoney(double amount) {
		mMoney += amount;
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
};


class Bank {
public:
	void addAccount(Account* account) {
		mAccounts.insert(account);
	}

	bool transferMoneyDeadblock(Account* accountA, Account* accountB, double amount) {

		//如果这样会发生死锁
		lock_guard<std::mutex> guardA(*accountA->getLock()); // ①
		lock_guard<std::mutex> guardB(*accountB->getLock());

		if (amount > accountA->getMoney()) { // ②
			return false;
		}

		accountA->changeMoney(-amount); // ③
		accountB->changeMoney(amount);
		return true;
	}

	bool transferMoney(Account* accountA, Account* accountB, double amount) {

		//先获取两把锁
		lock<std::mutex>(*(accountA->getLock()), *(accountB->getLock()));
		lock_guard<std::mutex> guardA(*accountA->getLock(), std::adopt_lock); // ①
		lock_guard<std::mutex> guardB(*accountB->getLock(), std::adopt_lock);

		if (amount > accountA->getMoney()) { // ②
			return false;
		}

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
		if (bank->transferMoney(accountA, accountB, randomMoney)) {
			print_mutex.lock();
			cout << "Transfer " << randomMoney << " from " << accountA->getName()
				<< " to " << accountB->getName()
				<< ", Bank totalMoney: " << bank->totalMoney() << endl;
			print_mutex.unlock();
		}

		
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

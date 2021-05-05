#pragma once
#include <atomic>
#include <mutex>
#include <string>
using namespace std;
class CashRegister
{
	int money;
	int id;
	int carCounter;
	string state;
	atomic<bool> blocked;
public:
	mutex mutex;
	bool Lock();
	bool Unlock();
	CashRegister(int id);
	void Pay(int x);
	string PrintState();
};


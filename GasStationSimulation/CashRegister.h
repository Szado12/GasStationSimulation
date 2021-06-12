#pragma once
#include <atomic>
#include <mutex>
#include <string>
using namespace std;
struct CashRegisterState{
	bool state;
	int money;
	int carCounter;
};
class CashRegister
{
	int money;
	int id;
	int carCounter;
	atomic<bool> blocked;
public:
	mutex mutexCashRegister;
	bool Lock();
	bool Unlock();
	CashRegister(int id);
	void Pay(int x);
	int getId();
	bool Free();
	CashRegisterState PrintState();
};


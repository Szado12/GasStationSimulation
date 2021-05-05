#pragma once
#include <atomic>
#include <mutex>
#include <algorithm>
using namespace std;
class Distributor
{
	int id;
	atomic<bool> blocked;
	atomic<bool> empty;
	int fuel;
	int maxFuel;
public:
	mutex mutex;
	Distributor(int id);
	string PrintState();
	bool Lock();
	bool Unlock();
	bool IsEmpty();
	int FuelTaken(int fuel);
	void RefuelDistributor();

};


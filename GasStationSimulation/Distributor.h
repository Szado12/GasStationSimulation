#pragma once
#include <atomic>
#include <mutex>
#include <algorithm>
using namespace std;
struct DistributorState
{
	int state;
	int fuel;
};

class Distributor
{
	atomic<int> id;
	atomic<bool> blocked;
	atomic<bool> empty;
	atomic<int> fuel;
	int maxFuel;
public:
	mutex mutexDistributor;
	Distributor(int id);
	DistributorState PrintState();
	int getId();
	bool Lock();
	bool Unlock();
	bool IsEmpty();
	bool IsLocked();
	int FuelTaken(int fuel);
	void RefuelDistributor();

};


class Simulation;
#pragma once
#include "Distributor.h"
#include <atomic>
class TankTruck
{
private:
	Distributor* usedDistributor;
	atomic<bool> blocked;
	int driveTime;
	int id;
	string status;
	int refuelTime;
	int refuelTruckTime;
	Simulation* simulation;
public:
	mutex mutex;
	TankTruck(int id, int driveTime, int refuelTime, int refuelTruckTime, Simulation* simulation);
	bool IsBlocked();
	string PrintStatus();
	void CallTruck(Distributor* usedDistributor);
	void Simulate();
	void Drive();
	void RefuelDistributor();
	void RefuelTruck();
	void Wait();
};


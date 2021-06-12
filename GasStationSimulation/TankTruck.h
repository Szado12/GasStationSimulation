class Simulation;
#pragma once
#include "DistributorManager.h"
#include "Distributor.h"
#include <atomic>


struct statusTruck {
	int id;
	int state;
	int distributorId;
};
class TankTruck
{
private:
	Distributor* usedDistributor;
	atomic<bool> blocked;
	atomic<int> state;
	int id;
	int driveTime;
	int refuelTime;
	int refuelTruckTime;
	DistributorManager* distributorManager;
	atomic<bool> continueSimulation;
public:
	mutex mutexTruck;
	TankTruck(int id, int driveTime, int refuelTime, int refuelTruckTime, DistributorManager* distributorManager);
	bool IsBlocked();
	statusTruck PrintStatus();
	void Simulate();
	void Drive();
	void DriveBack();
	void RefuelDistributor();
	void RefuelTruck();
	void Wait();
	void EndSimulation();
};


#pragma once
#include "Distributor.h"
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <queue>
#include <vector>
class DistributorManager
{
	queue<Distributor*>* emptyDistributors;
	vector<Distributor*>* distributors;
	vector<Distributor*>* allDistributors;
public:
	atomic<bool> free;
	atomic<bool> empty;
	DistributorManager(int count);
	mutex mutexDistributorManager;
	condition_variable condVarDistributorManager;
	condition_variable condVarDistributorManagerEmpty;
	Distributor* AssignDistributorToCar(int id);
	Distributor* AssignDistributorToTruck();
	void ReturnDistributorCar(Distributor* distributor);
	void ReturnDistributorTruck(Distributor* distributor);
	bool CheckIsDistributorFree();
	vector<DistributorState> printState();
};


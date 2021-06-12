class Simulation;
#pragma once
#include <algorithm>
#include "Distributor.h"
#include "CashRegister.h"
#include "DistributorManager.h"
#include "CashRegisterManager.h"
#include <atomic>

struct statusCar {
	int distributorId;
	int cashRegisterId;
	int status;
	int id;
};

class Car
{
	int id;
	int fuel;
	int maxFuel;
	int refueledFuel;
	atomic<int> state;
	DistributorManager* distributorManager;
	CashRegisterManager* cashRegisterManager;
	Distributor* usedDistributor;
	CashRegister* usedCashRegister;
	int driveTime;
	int refuelTime;
	int payingTime;
	atomic<bool> continueSimulation;
public:
	mutex mutexCar;
	Car(int id,int driveTime, int refuelTime, int payingTime, int fuel, DistributorManager* distributorManager, CashRegisterManager* cashRegisterManager);
	void Refuel();
	void Pay();
	statusCar PrintStatus();
	void Simulate();
	void Drive();
	void EndSimulation();
};


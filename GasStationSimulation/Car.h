class Simulation;
#pragma once
#include <algorithm>
#include "Distributor.h"
#include "CashRegister.h"
#include <atomic>

class Car
{
	int id;
	int fuel;
	int maxFuel;
	int refueledFuel;
	string state;
	Simulation* simulation;
	Distributor* usedDistributor;
	CashRegister* usedCashRegister;
	int driveTime;
	int refuelTime;
	int payingTime;
	int payingCounter;
	int refuelingCounter;
	atomic<bool> continueSimulation;
public:
	Car(int id,int driveTime, int refuelTime, int payingTime, int fuel, Simulation* simulation);
	void Refuel();
	void Pay();
	void WaitingForDistributor();
	void WaitingForCashRegister();
	string PrintStatus();
	void setDistributor(Distributor* usedDistributor);
	void setCashRegister(CashRegister* usedCashRegister);
	void Simulate();
	void Drive();
	void EndSimulation();
};


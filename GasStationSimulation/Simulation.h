#pragma once
#include "Distributor.h"
#include <vector>
#include <queue>
#include <thread>
#include "Car.h"
#include "Distributor.h"
#include "TankTruck.h"

class Simulation
{
	vector<Car*>* cars;
	vector<Distributor*>* distributors;
	vector<CashRegister*>* cashRegisters;
	vector<TankTruck*>* tankTrucks;
	vector<thread*>* carThreads;
	vector<thread*>* truckThreads;
	queue<Car*>* refuelQueue;
	queue<Distributor*>* emptyDistributorsQueue;
	queue<Car*>* payQueue;
	bool simulate;

public:
	mutex mutex;
	void AssignDistributorToCar();
	void AssignCashRegisterToCar();
	void StartSimulation();
	Simulation();
	void Print();
	void RefuelDistributor(Distributor* distributor);
	void CallTrucks();
	void AddCarToRefuelQueue(Car* car);
	void AddCarToPayQueue(Car* car);
};


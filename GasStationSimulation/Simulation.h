#pragma once
#include "Distributor.h"
#include <vector>
#include <queue>
#include <thread>
#include <atomic>
#include <ncurses.h>
#include "DistributorManager.h"
#include "CashRegisterManager.h"
#include "Car.h"
#include "Distributor.h"
#include "TankTruck.h"

class Simulation
{

	vector<Car*>* cars;
	DistributorManager* distributorManager;
	CashRegisterManager* cashRegisterManager;
	vector<TankTruck*>* tankTrucks;
	vector<thread*>* carThreads;
	vector<thread*>* truckThreads;
	queue<Car*>* payQueue;
	atomic<bool> simulate;

public:
	mutex mutexSimulation;
	void DrawBg();
	void InitialColors();
	void StartSimulation();
	Simulation(int carsCount,int carDriveTime,int carRefuelTime,int carPayTime, int trucksCount,int truckDriveTime,int truckRefuelTime,int truckRefuelDTime,int distributorsCount, int cashRegCount);
	void Print();
	void EndSimulation();
};


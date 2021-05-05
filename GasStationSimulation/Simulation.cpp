#include "Simulation.h"
#include <string>
#include <iostream>
#include <windows.h>
#include "CashRegister.h"

Simulation::Simulation()
{
	cars = new vector<Car*>();
	distributors = new vector<Distributor*>();
	cashRegisters = new vector<CashRegister*>();
	refuelQueue = new queue<Car*>();
	tankTrucks = new vector<TankTruck*>();
	payQueue = new queue<Car*>();
	carThreads = new vector<thread*>();
	truckThreads = new vector<thread*>();
	emptyDistributorsQueue = new queue<Distributor*>();
	simulate = true;
	for (int i = 0; i < 10; i++) 
	{
		cars->push_back(new Car(i, 500, 500, 500, 200, this));
	}
	for (int i = 0; i < 6; i++) 
	{
		distributors->push_back(new Distributor(i));
	}
	for (int i = 0; i < 3; i++)
	{
		cashRegisters->push_back(new CashRegister(i));
	}
	for (int i = 0; i < 3; i++)
	{
		tankTrucks->push_back(new TankTruck(i,2000,2000,2000,this));
	}
}

void Simulation::AssignDistributorToCar()
{
	
	for (int j = 0; j < distributors->size(); j++) 
	{
		if (distributors->at(j)->mutex.try_lock())
		{
			if (distributors->at(j)->IsEmpty()) {
				distributors->at(j)->mutex.unlock();
				emptyDistributorsQueue->push(distributors->at(j));
				distributors->erase(distributors->begin() + j);
			}
			else
			{
				if (distributors->at(j)->Lock())
				{
					Car* car = refuelQueue->front();
					car->setDistributor(distributors->at(j));
					distributors->at(j)->mutex.unlock();
					refuelQueue->pop();
					break;
				}
				distributors->at(j)->mutex.unlock();
			}
		}
	}
}
void Simulation::AssignCashRegisterToCar()
{
	for (int j = 0; j < cashRegisters->size(); j++)
	{
		if (cashRegisters->at(j)->mutex.try_lock())
		{
			
			if (cashRegisters->at(j)->Lock())
			{
				Car* car = payQueue->front();
				car->setCashRegister(cashRegisters->at(j));
				cashRegisters->at(j)->mutex.unlock();
				payQueue->pop();
				break;
			}
			cashRegisters->at(j)->mutex.unlock();
			
		}
	}
}
void Simulation::Print() {
	string info = "";
	while (simulate) {
		info = "";
		this_thread::sleep_for(chrono::milliseconds(1000));
		try
		{
			for (int i = 0; i < cars->size(); i++)
			{
				info += cars->at(i)->PrintStatus();
			}
			for (int i = 0; i < distributors->size(); i++)
			{
				info += distributors->at(i)->PrintState();
			}
			for (int i = 0; i < cashRegisters->size(); i++)
			{
				info += cashRegisters->at(i)->PrintState();
			}
			for (int i = 0; i < tankTrucks->size(); i++)
			{
				info += tankTrucks->at(i)->PrintStatus();
			}
		}
		catch (const std::exception&)
		{
			info = "aaaa";
		}
		
		system("cls");
		cout << info << endl;
	}
}
void Simulation::RefuelDistributor(Distributor* distributor)
{
	this->distributors->push_back(distributor);
}
void Simulation::CallTrucks()
{
	for (int i = 0; i < tankTrucks->size(); i++) {
		if (tankTrucks->at(i)->mutex.try_lock()) {
			if (!tankTrucks->at(i)->IsBlocked()) {
				Distributor* emptyDistributor = emptyDistributorsQueue->front();
				emptyDistributorsQueue->pop();
				tankTrucks->at(i)->CallTruck(emptyDistributor);
				tankTrucks->at(i)->mutex.unlock();
				break;
			}
			tankTrucks->at(i)->mutex.unlock();
		}
	}
}
void Simulation::StartSimulation()
{
	for (int i = 0; i < cars->size(); i++) 
	{
		carThreads->push_back(new thread(&Car::Simulate, cars->at(i)));
	}
	for (int i = 0; i < tankTrucks->size(); i++)
	{
		truckThreads->push_back(new thread(&TankTruck::Simulate, tankTrucks->at(i)));
	}
	thread* printing = new thread(&Simulation::Print,this);

	for (int i = 0; i < 10000; i++) 
	{
		this_thread::sleep_for(chrono::milliseconds(100));
		if (!refuelQueue->empty()) 
			AssignDistributorToCar();
		if(!payQueue->empty())
			AssignCashRegisterToCar();
		if (!emptyDistributorsQueue->empty())
			CallTrucks();
	}
	for (int i = 0; i < cars->size(); i++)
	{
		cars->at(i)->EndSimulation();
	}
	for (int i = 0; i < cars->size(); i++)
	{
		carThreads->at(i)->join();
	}
}

void Simulation::AddCarToRefuelQueue(Car* car)
{
	refuelQueue->push(car);
}

void Simulation::AddCarToPayQueue(Car* car)
{
	payQueue->push(car);
}

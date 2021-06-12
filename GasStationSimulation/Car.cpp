#include "Car.h"
#include "Simulation.h"
#include <string>
#include <iostream>


Car::Car(int id, int driveTime, int refuelTime, int payingTime, int fuel, DistributorManager* distributorManager, CashRegisterManager* cashRegisterManager)
{
	this->continueSimulation = true;
	this->id = id;
	this->driveTime = driveTime;
	this->refuelTime = refuelTime;
	this->payingTime = payingTime;
	this->fuel = fuel;
	this->maxFuel = fuel;
	this->refueledFuel = 0;
	this->distributorManager = distributorManager;
	this->cashRegisterManager = cashRegisterManager;
	this->usedDistributor = nullptr;
}

void Car::Refuel()
{
	
	state = 1;
	std::unique_lock<std::mutex> lk(distributorManager->mutexDistributorManager);
	while (!distributorManager->free and continueSimulation) distributorManager->condVarDistributorManager.wait(lk);
	if(!continueSimulation){
		lk.unlock();
		return;
	}
	usedDistributor = distributorManager->AssignDistributorToCar(this->id);
	lk.unlock();
	state = 2;
	usedDistributor->mutexDistributor.lock();
	this->refueledFuel = usedDistributor->FuelTaken(this->maxFuel - this->fuel);
	usedDistributor->mutexDistributor.unlock();
	this->fuel += this->refueledFuel;
	this_thread::sleep_for(chrono::milliseconds(this->refuelTime + int((float(rand()%40) -20.0)/100.0*this->refuelTime)));
	
}

void Car::Pay()
{
	state = 3;
	std::unique_lock<std::mutex> lk(cashRegisterManager->mutexCashRegisterManager);
	while (!cashRegisterManager->free and continueSimulation) cashRegisterManager->condVarCashRegisterManager.wait(lk);
	if(!continueSimulation){
		lk.unlock();
		return;
	}
	usedCashRegister = cashRegisterManager->AssignCashToCar();
	lk.unlock();
	state = 4;
	this_thread::sleep_for(chrono::milliseconds(this->payingTime + int((float(rand()%40) -20.0)/100.0*this->payingTime)));
	this->usedDistributor->mutexDistributor.lock();
	this->usedDistributor->Unlock();
	this->usedDistributor->mutexDistributor.unlock();
	this->usedCashRegister->mutexCashRegister.lock();
	this->usedCashRegister->Pay(this->refueledFuel);
	this->usedCashRegister->Unlock();
	this->usedCashRegister->mutexCashRegister.unlock();
	this->refueledFuel = 0;
	distributorManager->mutexDistributorManager.lock();
	distributorManager->ReturnDistributorCar(usedDistributor);
	distributorManager->mutexDistributorManager.unlock();
	cashRegisterManager->mutexCashRegisterManager.lock();
	cashRegisterManager->ReturnCashRegister(usedCashRegister);
	cashRegisterManager->mutexCashRegisterManager.unlock();
	usedDistributor = nullptr;
	usedCashRegister = nullptr;
}


statusCar Car::PrintStatus()
{
	statusCar a;
	a.id = this->id;
	a.status = this->state;
	a.distributorId = 0;
	a.cashRegisterId = 0;
	if (this->usedDistributor != nullptr)
		a.distributorId = usedDistributor->getId();
	if (this->usedCashRegister != nullptr)
		a.cashRegisterId = usedCashRegister->getId();
	return a;
}


void Car::Simulate()
{
	while (this->continueSimulation) {
		Drive();
		Refuel();
		Pay();
	}
}

void Car::Drive()
{
	state = 0;
	this_thread::sleep_for(chrono::milliseconds(this->driveTime + int((float(rand()%40) -20.0)/100.0*this->driveTime)));
	fuel = max(0, (fuel - rand()%maxFuel));
}

void Car::EndSimulation()
{
	this->continueSimulation = false;
}


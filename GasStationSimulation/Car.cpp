#include "Car.h"
#include "Simulation.h"
#include <string>

Car::Car(int id, int driveTime, int refuelTime, int payingTime, int fuel, Simulation* simulation)
{
	this->continueSimulation = true;
	this->id = id;
	this->driveTime = driveTime;
	this->refuelTime = refuelTime;
	this->payingTime = payingTime;
	this->fuel = fuel;
	this->maxFuel = fuel;
	this->refueledFuel = 0;
	this->simulation = simulation;
	usedDistributor = nullptr;
}

void Car::Refuel()
{
	state = "Waiting";
	while(usedDistributor == nullptr and continueSimulation==true)
		this_thread::sleep_for(chrono::milliseconds(100));
	state = "Refueling";
	usedDistributor->mutex.lock();
	this->refueledFuel = usedDistributor->FuelTaken(this->maxFuel - this->fuel);
	usedDistributor->mutex.unlock();
	this->fuel += this->refueledFuel;
	this_thread::sleep_for(chrono::milliseconds(this->refuelTime));
	
}

void Car::Pay()
{
	state = "Waiting";
	while (usedCashRegister == nullptr and continueSimulation == true)
		this_thread::sleep_for(chrono::milliseconds(100));
	this->usedCashRegister->mutex.lock();
	state = "Paying";
	this_thread::sleep_for(chrono::milliseconds(this->payingTime));
	this->usedDistributor->mutex.lock();
	this->usedDistributor->Unlock();
	this->usedDistributor->mutex.unlock();
	this->usedCashRegister->Pay(this->refueledFuel);
	this->usedCashRegister->Unlock();
	this->usedCashRegister->mutex.unlock();
	this->refueledFuel = 0;
	usedDistributor = nullptr;
	usedCashRegister = nullptr;
}

void Car::WaitingForDistributor()
{
	this->simulation->mutex.lock();
	this->simulation->AddCarToRefuelQueue(this);
	this->simulation->mutex.unlock();
}

void Car::WaitingForCashRegister()
{
	this->simulation->mutex.lock();
	this->simulation->AddCarToPayQueue(this);
	this->simulation->mutex.unlock();
}

string Car::PrintStatus()
{
	return  "CarId: " + to_string(id) + " State: " + state + "\n";
}

void Car::setDistributor(Distributor* usedDistributor)
{
	this->usedDistributor = usedDistributor;
}

void Car::setCashRegister(CashRegister* usedCashRegister)
{
	this->usedCashRegister = usedCashRegister;
}

void Car::Simulate()
{
	while (this->continueSimulation) {
		Drive();
		WaitingForDistributor();
		Refuel();
		WaitingForCashRegister();
		Pay();
	}
}

void Car::Drive()
{
	state = "Driving";
	this_thread::sleep_for(chrono::milliseconds(this->driveTime));
	fuel = max(0, (fuel - rand()%maxFuel));
}

void Car::EndSimulation()
{
	this->continueSimulation = false;
}


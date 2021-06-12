#include "Distributor.h"
#include <string>
Distributor::Distributor(int id)
{
	this->id = id;
	this->fuel = 2000;
	this->maxFuel = 2000;
	this->empty = false;
}

DistributorState Distributor::PrintState()
{
	DistributorState a;
	if (blocked)
		a.state = 1;
	else if (empty)
		a.state = 2;
	else
		a.state = 0;
	a.fuel = this->fuel;
	return a;
}

int Distributor::getId()
{
	return this->id;
}

bool Distributor::Lock()
{
	if (blocked == true)
		return false;
	blocked = true;
	return true;
}

bool Distributor::Unlock()
{
	if (blocked == false)
		return false;
	blocked = false;
	return true;
}

bool Distributor::IsEmpty()
{
	return this->empty;
}

bool Distributor::IsLocked()
{
	return blocked;
}

int Distributor::FuelTaken(int fuel)
{
	int fuelTaken = fuel;
	if(fuel > this->fuel)
		fuelTaken = this->fuel;
	this->fuel -= fuelTaken;
	if (this->fuel == 0)
		this->empty = true;
	return fuelTaken;
}

void Distributor::RefuelDistributor()
{
	fuel = maxFuel;
	empty = false;
}

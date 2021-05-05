#include "Distributor.h"
#include <string>
Distributor::Distributor(int id)
{
	this->id = id;
	this->fuel = 2000;
	this->maxFuel = 2000;
	this->empty = false;
}

string Distributor::PrintState()
{
	string print = "Distributor: " + to_string(id) + " state: ";
	if (blocked)
		print += "blocked ";
	else
		print += "free ";
	print += "Current Fuel: " + to_string(fuel)+"\n";
	return print;
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

int Distributor::FuelTaken(int fuel)
{
	int fuelTaken = min(fuel, this->fuel);
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

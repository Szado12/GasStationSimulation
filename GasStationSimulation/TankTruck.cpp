#include "TankTruck.h"
#include "Simulation.h"
#include <string>
TankTruck::TankTruck(int id,int driveTime, int refuelTime, int refuelTruckTime, Simulation* simulation)
{
	this->id = id;
	this->driveTime = driveTime;
	this->refuelTruckTime = refuelTruckTime;
	this->refuelTime = refuelTime;
	this->blocked = false;
	this->usedDistributor = nullptr;
	this->simulation = simulation;
}

bool TankTruck::IsBlocked()
{
	return blocked;
}

string TankTruck::PrintStatus()
{
	return "TankTruck " + to_string(this->id) + " status: " + this->status + "\n";
}

void TankTruck::CallTruck(Distributor* usedDistributor)
{
	blocked = true;
	this->usedDistributor = usedDistributor;
}

void TankTruck::Simulate()
{
	while (true) 
	{
		Wait();
		RefuelTruck();
		Drive();
		RefuelDistributor();
		Drive();
	}
}


void TankTruck::Drive()
{
	this->status = "drive";
	this_thread::sleep_for(chrono::milliseconds(this->driveTime));
}

void TankTruck::RefuelDistributor()
{
	this->status = "refueling distributor";
	usedDistributor->mutex.lock();
	usedDistributor->Lock();
	usedDistributor->RefuelDistributor();
	this_thread::sleep_for(chrono::milliseconds(this->refuelTime));
	usedDistributor->Unlock();
	simulation->mutex.lock();
	simulation->RefuelDistributor(usedDistributor);
	simulation->mutex.unlock();
	usedDistributor->mutex.unlock();
	blocked = false;
	usedDistributor = nullptr;
}

void TankTruck::RefuelTruck()
{
	this->status = "refueling truck";
	this_thread::sleep_for(chrono::milliseconds(this->refuelTruckTime));
}

void TankTruck::Wait()
{
	this->status = "wait";
	while(usedDistributor == nullptr)
		this_thread::sleep_for(chrono::milliseconds(500));
}

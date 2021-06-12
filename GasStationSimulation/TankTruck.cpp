#include "TankTruck.h"
#include "Simulation.h"
#include <string>
TankTruck::TankTruck(int id,int driveTime, int refuelTime, int refuelTruckTime, DistributorManager* distributorManager)
{
	this->id = id;
	this->driveTime = driveTime;
	this->refuelTruckTime = refuelTruckTime;
	this->refuelTime = refuelTime;
	this->blocked = false;
	this->continueSimulation = true;
	this->usedDistributor = nullptr;
	this->distributorManager = distributorManager;
}

bool TankTruck::IsBlocked()
{
	return blocked;
}

statusTruck TankTruck::PrintStatus()
{
	statusTruck a;
	a.id = this->id;
	a.distributorId = 0;
	if (this->usedDistributor != nullptr)
		a.distributorId = usedDistributor->getId();
	a.state = state;
	return a;
}



void TankTruck::Simulate()
{
	while (continueSimulation)
	{
		Wait();
		RefuelTruck();
		Drive();
		RefuelDistributor();
		DriveBack();
	}
}


void TankTruck::Drive()
{
	if (!continueSimulation)
		return;
	this->state = 2;
	this_thread::sleep_for(chrono::milliseconds(this->driveTime));
}
void TankTruck::DriveBack()
{
	if (!continueSimulation)
		return;
	this->state = 4;
	this_thread::sleep_for(chrono::milliseconds(this->driveTime));
	blocked = false;
}
void TankTruck::RefuelDistributor()
{
	if (!continueSimulation)
		return;
	this->state = 3;
	usedDistributor->mutexDistributor.lock();
	usedDistributor->Lock();
	usedDistributor->RefuelDistributor();
	this_thread::sleep_for(chrono::milliseconds(this->refuelTime));
	usedDistributor->Unlock();
	usedDistributor->mutexDistributor.unlock();
	distributorManager->mutexDistributorManager.lock();
	distributorManager->ReturnDistributorTruck(usedDistributor);
	distributorManager->mutexDistributorManager.unlock();
	usedDistributor = nullptr;
}

void TankTruck::RefuelTruck()
{
	if (!continueSimulation)
		return;
	this->state = 1;
	this_thread::sleep_for(chrono::milliseconds(this->refuelTruckTime));
}

void TankTruck::Wait()
{
	this->state = 0;
	std::unique_lock<std::mutex> lk(distributorManager->mutexDistributorManager);
	while (!distributorManager->empty and continueSimulation) distributorManager->condVarDistributorManagerEmpty.wait(lk);
	if(!continueSimulation)
		return;
	this->usedDistributor = distributorManager->AssignDistributorToTruck();
	lk.unlock();
}

void TankTruck::EndSimulation()
{
	this->continueSimulation = false;
}

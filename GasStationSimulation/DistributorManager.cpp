#include "DistributorManager.h"

DistributorManager::DistributorManager(int count)
{
	distributors = new vector<Distributor*>();
	allDistributors = new vector<Distributor*>();
	emptyDistributors = new queue<Distributor*>();
	for (int i = 0; i < count; i++)
	{
		distributors->push_back(new Distributor(i));
		allDistributors->push_back(distributors->at(i));
	}
	empty = false;
	free = true;
}

Distributor* DistributorManager::AssignDistributorToCar(int id)
{
	
	for (int i = 0; i < distributors->size(); i++)
	{
		if (distributors->at(i)->Lock()){
			free = CheckIsDistributorFree();
			return distributors->at(i);
		}
	}
	free = CheckIsDistributorFree();
	return nullptr;
}

Distributor* DistributorManager::AssignDistributorToTruck()
{
	Distributor* empty = emptyDistributors->front();
	emptyDistributors->pop();
	if (emptyDistributors->empty())
		this->empty = false;
	return empty;
}

void DistributorManager::ReturnDistributorCar(Distributor* distributor)
{
	if (distributor->IsEmpty()) {
		emptyDistributors->push(distributor);
		empty = true;
		condVarDistributorManagerEmpty.notify_one();
		distributors->erase(remove(distributors->begin(), distributors->end(), distributor), distributors->end());
	}
	else {
		free = true;
		condVarDistributorManager.notify_one();
	}
}

void DistributorManager::ReturnDistributorTruck(Distributor* distributor)
{
	this->distributors->push_back(distributor);
	free = true;
	condVarDistributorManager.notify_one();

}

bool DistributorManager::CheckIsDistributorFree()
{
	for (int i = 0; i < distributors->size(); i++)
	{
		if (!distributors->at(i)->IsEmpty() && !distributors->at(i)->IsLocked())
			return true;
	}
	return false;
}

vector<DistributorState> DistributorManager::printState()
{
	vector<DistributorState> distributorsState;
	for (int i = 0; i < allDistributors->size(); i++)
	{
		distributorsState.push_back(allDistributors->at(i)->PrintState());
	}
	return distributorsState;
}

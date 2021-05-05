#include "CashRegister.h"


bool CashRegister::Lock()
{
	if (!this->blocked)
	{
		this->blocked = true;
		this->state = "In use";
		return true;
	}
	return false;
}

bool CashRegister::Unlock()
{
	if (this->blocked)
	{
		this->blocked = false;
		this->state = "free";
		return true;
	}
	return false;
}

CashRegister::CashRegister(int id)
{
	this->id = id;
	this->money = 0;
	this->carCounter = 0;
	this->state = "free";
}

void CashRegister::Pay(int x)
{
	this->money += x;
	carCounter++;
}

string CashRegister::PrintState()
{
	return "CashRegister: " + to_string(id) + " Status: " + state + " Cars: " + to_string(carCounter) + " Money: " + to_string(money) + "\n";
}


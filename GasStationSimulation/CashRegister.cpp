#include "CashRegister.h"


bool CashRegister::Lock()
{
	if (!this->blocked)
	{
		this->blocked = true;
		return true;
	}
	return false;
}

bool CashRegister::Unlock()
{
	if (this->blocked)
	{
		this->blocked = false;
		return true;
	}
	return false;
}

CashRegister::CashRegister(int id)
{
	this->id = id;
	this->money = 0;
	this->carCounter = 0;
}

void CashRegister::Pay(int x)
{
	this->money += x;
	carCounter++;
}

int CashRegister::getId()
{
	return id;
}

bool CashRegister::Free()
{
	return !blocked;
}

CashRegisterState CashRegister::PrintState()
{
	CashRegisterState x;
	x.carCounter = this->carCounter;
	x.money = this->money;
	x.state = this->blocked;
	return x;
}


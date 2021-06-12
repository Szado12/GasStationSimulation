#pragma once
#include "CashRegister.h"
#include <condition_variable>
#include <mutex>
#include <atomic>
#include <vector>


class CashRegisterManager
{
	vector<CashRegister*>* cashRegisters;
public:
	  atomic<bool> free;
	  CashRegisterManager(int count);
	  mutex mutexCashRegisterManager;
	  condition_variable condVarCashRegisterManager;
	  CashRegister* AssignCashToCar();
	  void ReturnCashRegister(CashRegister* cashRegister);
	  bool CheckIsCashRegisterFree();
	  vector<CashRegisterState> printState();
};


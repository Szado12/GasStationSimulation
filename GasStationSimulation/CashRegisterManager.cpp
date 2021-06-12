#include "CashRegisterManager.h"

CashRegisterManager::CashRegisterManager(int count)
{
    cashRegisters = new vector<CashRegister*>();
    for (int i = 0; i < count; i++) {
        cashRegisters->push_back(new CashRegister(i));
    }
    free = true;
}

CashRegister* CashRegisterManager::AssignCashToCar()
{
    for (int i = 0; i < cashRegisters->size(); i++)
    {
        if (cashRegisters->at(i)->Lock()){
            free = CheckIsCashRegisterFree();
            return cashRegisters->at(i);
        }
    }
    free = CheckIsCashRegisterFree();
    return nullptr;
}

void CashRegisterManager::ReturnCashRegister(CashRegister* cashRegister)
{
    free = true;
    condVarCashRegisterManager.notify_one();
}

bool CashRegisterManager::CheckIsCashRegisterFree()
{
    for (int i = 0; i < cashRegisters->size(); i++)
    {
        if (cashRegisters->at(i)->Free())
            return true;
    }
    return false;
}
vector<CashRegisterState> CashRegisterManager::printState()
{
    vector<CashRegisterState> cashState;
    for (int i = 0; i < cashRegisters->size(); i++)
    {
        cashState.push_back(cashRegisters->at(i)->PrintState());
    }
    return cashState;
}

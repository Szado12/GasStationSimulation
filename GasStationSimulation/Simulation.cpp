#include "Simulation.h"
#include <string>
#include <iostream>
#include "CashRegister.h"
#define CAR_DRIVE 1
#define CAR_WAIT 2
#define CAR_REFUEL 3
#define CAR_PAY 4
#define TRUCK_DRIVE 5
#define TRUCK_WAIT 6
#define TRUCK_REFUEL_D 7
#define TRUCK_REFUEL 8
#define DISTR_FREE 9
#define DISTR_USED 10
#define DISTR_EMPTY 11
#define CASH_FREE 12
#define CASH_USED 13
#define ROAD 14
#define GRASS 15
Simulation::Simulation(int carsCount,int carDriveTime,int carRefuelTime,int carPayTime, int trucksCount,int truckDriveTime,int truckRefuelTime,int truckRefuelDTime,int distributorsCount, int cashRegCount)
{
	cars = new vector<Car*>();
	tankTrucks = new vector<TankTruck*>();
	payQueue = new queue<Car*>();
	carThreads = new vector<thread*>();
	truckThreads = new vector<thread*>();
	simulate = true;
	distributorManager = new DistributorManager(distributorsCount);
	cashRegisterManager = new CashRegisterManager(cashRegCount);
	for (int i = 0; i < carsCount; i++)
	{
		cars->push_back(new Car(i, carDriveTime, carRefuelTime, carRefuelTime, 200, distributorManager,cashRegisterManager));
	}
	for (int i = 0; i < trucksCount; i++)
	{
		tankTrucks->push_back(new TankTruck(i,truckDriveTime,truckRefuelDTime,truckRefuelTime,distributorManager));
	}
}


void Simulation::DrawBg(){
	bkgd(COLOR_PAIR(GRASS));
    attron(COLOR_PAIR(ROAD));
    for(int i=0;i<12;i++){
        mvhline(i,0,' ',COLS);
    }
    for(int i=0;i<3;i++){
        mvhline(20+i,0,' ',COLS);
    }
    for(int i=0;i<10;i++){
        mvhline(15+i,30,' ',COLS);
    }
}
void Simulation::InitialColors(){
	init_pair(CAR_DRIVE,COLOR_WHITE,COLOR_BLUE);
    init_pair(CAR_WAIT,COLOR_YELLOW,COLOR_BLUE);
    init_pair(CAR_REFUEL,COLOR_RED,COLOR_BLUE);
    init_pair(CAR_PAY,COLOR_GREEN,COLOR_BLUE);
    init_pair(TRUCK_DRIVE,COLOR_WHITE,COLOR_CYAN);
    init_pair(TRUCK_WAIT,COLOR_YELLOW,COLOR_CYAN);
    init_pair(TRUCK_REFUEL_D,COLOR_MAGENTA,COLOR_CYAN);
    init_pair(TRUCK_REFUEL,COLOR_BLUE,COLOR_CYAN);
    init_pair(DISTR_FREE,COLOR_GREEN,COLOR_YELLOW);
    init_pair(DISTR_USED,COLOR_MAGENTA,COLOR_YELLOW);
    init_pair(DISTR_EMPTY,COLOR_RED,COLOR_YELLOW);
    init_pair(CASH_FREE,COLOR_GREEN,COLOR_WHITE);
    init_pair(CASH_USED,COLOR_YELLOW,COLOR_WHITE);
    init_pair(ROAD,COLOR_WHITE,COLOR_BLACK);
    init_pair(GRASS,COLOR_WHITE,COLOR_GREEN);
}
void Simulation::Print() {
	int counter =0;
	start_color();
    InitialColors();
	while (simulate) {
		counter++;
		clear();
		DrawBg();
		vector<statusCar> carsStatus;
		vector<statusTruck> trucksStatus;
		vector<DistributorState> distributorsState = distributorManager->printState();
		vector<CashRegisterState> cashRegistersState = cashRegisterManager->printState();
		for (int i = 0; i < cars->size(); i++) {
			carsStatus.push_back(cars->at(i)->PrintStatus());
		}
		for (int i = 0; i < tankTrucks->size(); i++) {
			trucksStatus.push_back(tankTrucks->at(i)->PrintStatus());
		}
		for(int i=0;i<carsStatus.size();i++){
			switch (carsStatus.at(i).status)
			{
			case 0:
				attron(COLOR_PAIR(CAR_DRIVE));
				move(i%10,(i+counter)%COLS);
				printw("[C%d]",i);
				break;
			case 2:
				attron(COLOR_PAIR(CAR_REFUEL));
				move(15+carsStatus.at(i).distributorId%9,34+15*(carsStatus.at(i).distributorId/9));
				printw("[C%d]",i);
				break;
			case 3:
				attron(COLOR_PAIR(CAR_WAIT));
				move(15+carsStatus.at(i).distributorId%9,46+15*(carsStatus.at(i).distributorId/9));
				printw("[C%d]",i);
				break;
			case 4:
				attron(COLOR_PAIR(CAR_PAY));
				move(15+carsStatus.at(i).cashRegisterId%9,74+15*(carsStatus.at(i).cashRegisterId/9));
				printw("[C%d]",i);
				break;
			default:
				break;
			}
		}
		move(20,0);
		int waitingCars=0;
		for(int i=0;i<carsStatus.size();i++){
			if(carsStatus.at(i).status==1){
				attron(COLOR_PAIR(CAR_WAIT));
				printw("[C%d]",i);
				attron(COLOR_PAIR(ROAD));
				printw(" ");
				if(waitingCars%4==0 and waitingCars!=0)
					move(20+i/4,0);
				waitingCars++;
			}
		}
		for(int i=0;i<cashRegistersState.size();i++){
			if(cashRegistersState.at(i).state)
				attron(COLOR_PAIR(CASH_USED));
			else
				attron(COLOR_PAIR(CASH_FREE));
			move(15+i%9,80+15*(i/9));
			printw("[CR%d]",i);
			move(30,i*13);
			printw("[CR%d](%d$)",i,cashRegistersState.at(i).money);
		}
		for(int i=0;i<distributorsState.size();i++){
			switch (distributorsState.at(i).state)
			{
			case 0:
				attron(COLOR_PAIR(DISTR_FREE));
				break;
			case 1:
				attron(COLOR_PAIR(DISTR_EMPTY));
				break;
			case 2:
				attron(COLOR_PAIR(DISTR_USED));
				break;
			default:
				break;
			}
			move(15+i%9,40+15*(i/9));
			printw("[D%d]",i);
			move(32,i*10);
			printw("[D%d](%d)",i,distributorsState.at(i).fuel);
		}
		for(int i=0;i<trucksStatus.size();i++){
			switch (trucksStatus.at(i).state)
			{
			case 0:
				attron(COLOR_PAIR(TRUCK_WAIT));
				move(12,i*9);
				printw("[TT %d]",i);
				break;
			case 1:
				attron(COLOR_PAIR(TRUCK_REFUEL));
				move(12,i*9);
				printw("[TT %d]",i);
				break;
			case 2:
				attron(COLOR_PAIR(TRUCK_DRIVE));
				move(10,(10*i+counter/2)%COLS);
				printw("[TT %d]",i);
				break;
			case 3:
				attron(COLOR_PAIR(TRUCK_REFUEL_D));
				move(15+trucksStatus.at(i).distributorId%9,32+15*(trucksStatus.at(i).distributorId/9));
				printw("[TT %d]",i);
				break;
			case 4:
				attron(COLOR_PAIR(TRUCK_DRIVE));
				move(11,(COLS - (counter/2+i*10)%COLS));
				printw("[TT %d]",i);
				break;
			
			default:
				break;
			}
			
		}
		refresh();
		this_thread::sleep_for(chrono::milliseconds(100));
	}
}


void Simulation::StartSimulation()
{
	for (int i = 0; i < cars->size(); i++) 
	{
		carThreads->push_back(new thread(&Car::Simulate, cars->at(i)));
	}
	for (int i = 0; i < tankTrucks->size(); i++)
	{
		truckThreads->push_back(new thread(&TankTruck::Simulate, tankTrucks->at(i)));
	}
	
	initscr();
	thread* printing = new thread(&Simulation::Print,this);
	thread* end = new thread(&Simulation::EndSimulation,this);

	while (simulate) 
	{
		this_thread::sleep_for(chrono::milliseconds(100));

	}
	for (int i = 0; i < cars->size(); i++)
	{
		cars->at(i)->EndSimulation();
	}
	clear();
	refresh();
	for (int i = 0; i < cars->size(); i++)
	{
		carThreads->at(i)->join();
	}
	for (int i = 0; i < tankTrucks->size(); i++)
	{
		tankTrucks->at(i)->EndSimulation();
	}
	distributorManager->condVarDistributorManagerEmpty.notify_all();
	for (int i = 0; i < truckThreads->size(); i++)
	{
		truckThreads->at(i)->join();
	}
	end->join();
	printing->join();
    endwin();
}
void Simulation::EndSimulation(){
	while(getch() != 'q');
	this->simulate=false;
}

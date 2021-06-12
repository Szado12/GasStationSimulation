#include <iostream>
#include "Simulation.h"
int main()
{
    int cars=10, carDrivingTime=5000, carRefuelTime=2000, carPayingTime=2000;
    int trucks=4, truckDrivingTime=7000, truckRefuelTime=8000, truckRefuelDtime=8000;
    int distributors=8;
    int cashRegisters=5;
    srand(time(NULL));
    cout<<"Car data(count, car driving time, car refueling time, car paying time)"<<endl;
    cin >> cars >> carDrivingTime >> carRefuelTime >> carPayingTime;
    cout<<"Tank Truck data(count, truck driving time, truck refueling time, distributor refueling time)"<<endl;
    cin >> trucks >> truckDrivingTime >> truckDrivingTime >> truckRefuelDtime;
    cout<<"Distributors data(count)"<<endl;
    cin>>distributors;
    cout<<"Cash Register data(count)"<<endl;
    cin >> cashRegisters;
    Simulation* simulation = new Simulation(cars,carDrivingTime,carRefuelTime,carPayingTime,trucks,truckDrivingTime,truckRefuelTime,truckRefuelDtime,distributors,cashRegisters);
    simulation->StartSimulation();
}

//
// Created by Elin Fritiofsson on 2018-05-22.
//

#include <iostream>
#include <fstream>
#include "Station.h"


void Station::readVehicleFromStringstream(std::stringstream &ss){

    //set default values
    int id = -1;
    int type = -1;

    ss >> id;
    ss >> type;

    //if id and type have actually bin set to reasonable values.
    if(id!=-1 && type!=-1){
        switch(type){
            case 0: {
                int seats, internet;
                ss >> seats;
                ss >> internet;
                auto newVehicle = std::make_shared<DayCarriage>(id, seats, internet);
                //put it in the station queue
                vehiclesType0.push(newVehicle);
                //also put it in the allVehicles-queue.
                allVehicles.push_back(newVehicle);
            } break;
            case 1: {

                int beds;
                ss >> beds;
                auto newVehicle = std::make_shared<Sleeper>(id, beds);
                vehiclesType1.push(newVehicle);
                allVehicles.push_back(newVehicle);
            } break;
            case 2: {
                int capacity;
                int surface;
                ss >> capacity;
                ss >> surface;
                auto newVehicle = std::make_shared<OpenFreightCarriage>(id, capacity, surface);
                vehiclesType2.push(newVehicle);
                allVehicles.push_back(newVehicle);
            } break;
            case 3:{
                int volume;
                ss >> volume;
                auto newVehicle = std::make_shared<ClosedFreightCarriage>(id, volume);
                vehiclesType3.push(newVehicle);
                allVehicles.push_back(newVehicle);
            } break;
            case 4:{
                int maxSpeed, power;
                ss >> maxSpeed;
                ss >> power;
                auto newVehicle = std::make_shared<ElectricEngine>(id, maxSpeed, power);
                vehiclesType4.push(newVehicle);
                allVehicles.push_back(newVehicle);
            } break;
            case 5:{
                int maxSpeed, consumption;
                ss >> maxSpeed;
                ss >> consumption;
                auto newVehicle = std::make_shared<DieselEngine>(id, maxSpeed, consumption);
                vehiclesType5.push(newVehicle);
                allVehicles.push_back(newVehicle);
            } break;
            default: break;
        }
    }

    //append the new vehicle to a vector in the class..
}


std::vector<std::shared_ptr<Vehicle>> Station::getVehicles() {
    return allVehicles;
}


std::istream &operator>>(std::istream &is, std::unique_ptr<Station> &station) {


    std::string stationName;
    is >> stationName;
    std::string vehicles;
    getline(is, vehicles);
    std::stringstream ss(vehicles);
    std::string vehicle;


    while(std::getline(ss, vehicle, ')')){
        unsigned firstDelim = vehicle.find('(');
        std::string strNew = vehicle.substr(firstDelim+1, vehicle.size());
        std::stringstream vehicleStream(strNew);
        station->readVehicleFromStringstream(vehicleStream);
    }

    station->setName(stationName);


    return is;
}


void Station::deposeVehicle(std::shared_ptr<Vehicle> &returnedVehicle, int trainId, Time currentTime){

    //put vehicle in corresponding queue depending on its type value:
    switch(returnedVehicle->getType()){
        case 0: vehiclesType0.push(returnedVehicle);
            break;
        case 1: vehiclesType1.push(returnedVehicle);
            break;
        case 2: vehiclesType2.push(returnedVehicle);
            break;
        case 3: vehiclesType3.push(returnedVehicle);
            break;
        case 4: vehiclesType4.push(returnedVehicle);
            break;
        case 5: vehiclesType5.push(returnedVehicle);
            break;
        default: break;
    }

    //create a log message for the history log of the vehicle.
    std::string logMessage = currentTime.getTimeString() + "| ";
    logMessage += "Train " + std::to_string(trainId) + " returned vehicle at station: " + this->getName();

    //also push the vehicle to allvehicles-queue, independently of type.
    allVehicles.push_back(returnedVehicle);

    //log the state change message to the handled vehicle.
    returnedVehicle->logStateChange(logMessage);


}


bool Station::getVehicle(int vehicleType, std::shared_ptr<Vehicle> &vehicle, int trainId, Time currentTime){

    //make a pointer variable to a priority queue
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison>  *queue = nullptr;

    //make the variable point to the queue of the type we are looking for
    switch(vehicleType){
        case 0: queue = &vehiclesType0;
            break;
        case 1: queue = &vehiclesType1;
            break;
        case 2: queue = &vehiclesType2;
            break;
        case 3: queue = &vehiclesType3;
            break;
        case 4: queue = &vehiclesType4;
            break;
        case 5: queue = &vehiclesType5;
            break;
        default: break;
    }
    if(!queue->empty()){
        vehicle = queue->top();

        //we found such a vehicle, so we create a log message for taking the vehicle:
        std::string logMessage = currentTime.getTimeString() + "| ";
        logMessage += "Train " + std::to_string(trainId) + " took this vehicle at station: " + this->getName();
        vehicle->logStateChange(logMessage);
        //remove vehicle from the queue:
        queue->pop();

        //find this vehicle in the "all vehicles" queue:
        auto it = std::find_if(allVehicles.begin(), allVehicles.end(), [&vehicle](const std::shared_ptr<Vehicle> &v){
            return vehicle->getId()==v->getId();
        });
        //if we actually found it (which we should...) erase it from there as well.
        if(it!= allVehicles.end()){
            allVehicles.erase(it);
        };

    } else {
        //if queue was empty we return false.
        return false;

    }
    //we found the vehicle: return true.
    return true;
}
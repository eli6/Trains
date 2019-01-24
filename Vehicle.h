//
// Created by Elin Fritiofsson on 2018-05-22.
//

#ifndef UNTITLED1_VEHICLE_H
#define UNTITLED1_VEHICLE_H


#include <string>
#include <map>
#include <vector>

class Vehicle {
protected:
    int type;
    int id;
    std::string stateChangeLog; /**< string to hold a history of state changes for the vehicle */
public:
    virtual std::vector<std::pair<std::string, std::string>> getInfo() = 0; /**< base class has no info, only derived classes will */
    Vehicle() = default;
    Vehicle(int newId, int newType) : id(newId), type(newType) {};
    virtual ~Vehicle() {}; /**< virtual destructor */
    int getType() {return type;}; /**< get int representing the vehicle type */
    int getId() { return id; }
    void logStateChange(const std::string &newState); /**< add a new event to the stateChangeLog for documentation purposes */
    std::string getStateChangeHistory(){
        return stateChangeLog;
    }
};

//----------------------
// Child class Engine and derived classes
//----------------------
class Engine : public Vehicle {
protected:
    int maxSpeedInKmPerHour; /**< all derived engines share a max speed, so it is protected */
public:
    Engine() = default;
    Engine(int newId, int newMaxSpeed, int newType) : Vehicle(newId, newType){
            maxSpeedInKmPerHour = newMaxSpeed;
    };
};

class ElectricEngine : public Engine {
private:
    int powerIn_kW; /**< electric engine also has a powerInKw variable */
public:
    ElectricEngine() = default;
    ElectricEngine(int newId, int newMaxSpeed, int newPower) : Engine(newId, newMaxSpeed, 4){
        powerIn_kW = newPower;
    };
    std::vector<std::pair<std::string, std::string>> getInfo() override { /**< info specific for this class */
        std::vector<std::pair<std::string, std::string>> myVector;
        myVector.emplace_back(std::make_pair("Id", std::to_string(id)));
        myVector.emplace_back(std::make_pair("Type", "Electric Engine"));
        myVector.emplace_back(std::make_pair("Max speed in Km/h", std::to_string(maxSpeedInKmPerHour)));
        myVector.emplace_back(std::make_pair("Power in Kw", std::to_string(powerIn_kW)));
        return myVector;
    }
};

class DieselEngine : public Engine {
private:
    int fuelConsumptionInLiterPerHour; /**< diesel engine has a consumption variable */
public:
    DieselEngine() = default;
    DieselEngine(int newId, int newMaxSpeed, int newConsumption) : Engine(newId, newMaxSpeed , 5){
        fuelConsumptionInLiterPerHour = newConsumption;
    };
    std::vector<std::pair<std::string, std::string>> getInfo() override { /**< info specific for this class */
        std::vector<std::pair<std::string, std::string>> myVector;
        myVector.emplace_back(std::make_pair("Id", std::to_string(id)));
        myVector.emplace_back(std::make_pair("Type", "Diesel Engine"));
        myVector.emplace_back(std::make_pair("Max speed in Km/h", std::to_string(maxSpeedInKmPerHour)));
        myVector.emplace_back(std::make_pair("Fuel Consumption in l/h", std::to_string(fuelConsumptionInLiterPerHour)));
        return myVector;
    }
};


//----------------------
// (Empty) child class Carriage and derived classes
//----------------------
class Carriage : public Vehicle {
public:
    Carriage() = default;
    Carriage(int newId, int newType) : Vehicle(newId, newType){}; //behövs?
};


class Sleeper : public Carriage { //carriage with beds (sovvagn)
private:
    int numberOfBeds;
public:
    Sleeper() : Carriage(){};
    Sleeper(int newId, int newNrOfBeds) : Carriage(newId, 1), numberOfBeds(newNrOfBeds){};
    std::vector<std::pair<std::string, std::string>> getInfo() override { /**< info specific for this class */
        std::vector<std::pair<std::string, std::string>> myVector;
        myVector.emplace_back(std::make_pair("Id", std::to_string(id)));
        myVector.emplace_back(std::make_pair("Type", "Sleeper"));
        myVector.emplace_back(std::make_pair("Number of beds",std::to_string(numberOfBeds)));
        return myVector;
    }
};

class DayCarriage : public Carriage {  //carriage with seats (personvagn)
private:
    int numberOfSeats;
    bool internetAccess;
public:
    DayCarriage() = default;
    DayCarriage(int newId, int newNrOfSeats, bool newInternetAccess) : Carriage(newId, 0) {
        numberOfSeats = newNrOfSeats;
        internetAccess = newInternetAccess;
    }
    std::vector<std::pair<std::string, std::string>> getInfo() override { /**< info specific for this class */
        std::vector<std::pair<std::string, std::string>> myVector;
        myVector.emplace_back(std::make_pair("Id", std::to_string(id)));
        myVector.emplace_back(std::make_pair("Type", "Day Carriage"));
        myVector.emplace_back(std::make_pair("Number of seats", std::to_string(numberOfSeats)));
        if(internetAccess){ //make bool output suitable for printing
            myVector.emplace_back(std::make_pair("Internet Access", "yes"));
        } else {
            myVector.emplace_back(std::make_pair("Internet Access", "no"));
        }
        return myVector;
    }
};

class OpenFreightCarriage : public Carriage {
private:
    int capacityInTon;
    int spaceIn_m2;
public:
    OpenFreightCarriage() = default;
    OpenFreightCarriage(int newId, int newCapacity, int newSpaceIn_m2) : Carriage(newId, 2) {
        capacityInTon = newCapacity;
        spaceIn_m2 = newSpaceIn_m2;
    }
    std::vector<std::pair<std::string, std::string>> getInfo() override { /**< info specific for this class */
        std::vector<std::pair<std::string, std::string>> myVector;
        myVector.emplace_back(std::make_pair("Id", std::to_string(id)));
        myVector.emplace_back(std::make_pair("Type", "Open Freight Carriage"));
        myVector.emplace_back(std::make_pair("Capacity in ton",std::to_string(capacityInTon)));
        myVector.emplace_back(std::make_pair("Space in m2", std::to_string(spaceIn_m2)));
        return myVector;
    }
};

class ClosedFreightCarriage : public Carriage {
private:
    int volumeIn_m3;
public:
    ClosedFreightCarriage() = default;
    ClosedFreightCarriage(int newId, int newVolume) : Carriage(newId, 3), volumeIn_m3(newVolume) {}
    std::vector<std::pair<std::string, std::string>> getInfo() override { /**< info specific for this class */
        std::vector<std::pair<std::string, std::string>> myVector;
        myVector.emplace_back(std::make_pair("Id", std::to_string(id)));
        myVector.emplace_back(std::make_pair("Type", "Closed Freight Carriage"));
        myVector.emplace_back(std::make_pair("Volume (m3)", std::to_string(volumeIn_m3)));
        return myVector;
    }
};




#endif //UNTITLED1_VEHICLE_H

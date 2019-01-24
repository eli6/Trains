//
// Created by Elin Fritiofsson on 2018-05-22.
//

#ifndef UNTITLED1_STATION_H
#define UNTITLED1_STATION_H


#include "Vehicle.h"
#include "Time.h"
#include <sstream>
#include <vector>
#include <queue>
#include <memory>
#include <iostream>
#include <fstream>

class Station {
private:
    /**
     * Comparision function for vehicles in priority queues. lowest id first.
     */
    class VehicleIdComparison {
    public:
        bool operator() (std::shared_ptr<Vehicle> &left, std::shared_ptr<Vehicle> &right) {
            return left->getId() > right->getId();
        }
    };
    std::string name; /**< station name */
    std::vector<std::shared_ptr<Vehicle>> allVehicles; /**< all vehicles at this station */

    /**<priority queues for each vehicle type: */
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison> vehiclesType0;
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison> vehiclesType1;
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison> vehiclesType2;
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison> vehiclesType3;
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison> vehiclesType4;
    std::priority_queue<std::shared_ptr<Vehicle>, std::vector<std::shared_ptr<Vehicle>>, VehicleIdComparison> vehiclesType5;

public:
    Station() = default;
    //NEEDED BASIC SETTERS/GETTERS
    void setName(std::string &newName) { name = newName; }
    std::string getName() const { return name; }
    std::vector<std::shared_ptr<Vehicle>> getVehicles();
    //END OF SETTERS/GETTERS.

    /**< deposes a vehicle at station */
    void deposeVehicle(std::shared_ptr<Vehicle> &returnedVehicle, int trainId, Time currentTime);
    /**< gets a vehicle at station */
    bool getVehicle(int vehicleType, std::shared_ptr<Vehicle> &vehicle, int trainId, Time currentTime);
    /**< reads a vehicle from a stringstream */
    void readVehicleFromStringstream(std::stringstream &ss);
};

/**
 * Reads input stream from file for a station
 * @param is : input stream
 * @param station : station to be read in
 * @return the input stream
 */
std::istream &operator>> (std::istream &is, std::unique_ptr<Station> &station);

#endif //UNTITLED1_STATION_H

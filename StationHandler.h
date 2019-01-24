//
// Created by Elin Fritiofsson on 2018-05-25.
//

#ifndef NEWTRAINS_STATIONHANDLER_H
#define NEWTRAINS_STATIONHANDLER_H
#include "Station.h"
#include "Distance.h"
#include "Train.h"
#include <iostream>

class StationHandler {
private:
    std::shared_ptr<Station> station; /**< station to be handled */
    std::shared_ptr<StationList> stationList; /**< list of available stations */
public:
    StationHandler(std::shared_ptr<StationList> &newStationList) : stationList(newStationList){};
    std::vector<Distance> distances; /**< vector with distances between stations */
    void handle(const std::shared_ptr<Station> &newStation) {
        station = newStation; /**< sets the station to be handled */
    };

    /**
     * looks for vehicles for a certain train
     * @param train : train that is handled
     * @param currentTime : for logging purposes
     * @return bool : if all vehicles were found or not
     */
    bool findVehiclesFor(std::shared_ptr<Train> &train, Time currentTime);

    int getDistanceTo(std::shared_ptr<Station> destStation); /**< gets the distance to another station */
    void returnCarriagesFromTrain(std::shared_ptr<Train> &arrivedTrain, Time theTime); /**< returns carriages from a certain train to the station being handled*/
};
#endif //NEWTRAINS_STATIONHANDLER_H

//
// Created by Elin Fritiofsson on 2018-05-25.
//

#include "StationHandler.h"

void StationHandler::returnCarriagesFromTrain(std::shared_ptr<Train> &arrivedTrain, Time theTime){
    //go through the vehicles of the train and depose them at the station.
    for(auto &vehicle : arrivedTrain->getVehicles()){
        station->deposeVehicle(vehicle, arrivedTrain->getId(), theTime);
    }



}

bool StationHandler::findVehiclesFor(std::shared_ptr<Train> &train, Time currentTime) {
    bool foundAll = true; //variable to see if all vehicles were found
    std::shared_ptr<Vehicle> newVehicle;
    for (const auto &vehicle : train->getVehicleList()) {
        //try to get each vehicle for the train.
        if (!station->getVehicle(vehicle, newVehicle, train->getId(), currentTime)) {
            //if we cant find one vehicle, foundAll must be false.
            foundAll = false;
        } else {
            //if we found it, we add it to the train and removes it from the vehicle type list
            train->addVehicle(newVehicle);
            train->removeFromVehicleList(newVehicle->getType());
        }
    }
    //we return foundAll that indicates if the train is now complete or not.
    return foundAll;
}

int StationHandler::getDistanceTo(std::shared_ptr<Station> destStation) {

    auto it = std::find_if(distances.begin(), distances.end(), [this, &destStation](const Distance &distance) {

        //CHECKS DISTANCE FOR BOTH DIRECTIONS SINCE WE ONLY READ ONE FROM FILE.
        return (distance.getStartStation() == this->station && distance.getEndStation() == destStation
                || distance.getEndStation() == this->station && distance.getStartStation() == destStation);
    });
    //if we found the distance we return the distance, otherwise we return -1.
    if(it!=distances.end()){
        return it->getDistance();
    } else {
        return -1;
    }

}
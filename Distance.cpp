//
// Created by Elin Fritiofsson on 2018-05-28.
//

#include "Distance.h"

std::istream &operator>>(std::istream &is, std::unique_ptr<Distance> &distance) {

    std::string station1;
    std::string station2;

    is >> station1;
    is >> station2;

    //creates a new start station that should point to an existing station in the program
    std::shared_ptr<Station> startStation;
    //looks for the station in the station list:
    startStation = distance->getStationList()->findStation(station1);

    //if station was found, set the start station to that station
    if(startStation != nullptr){
        distance->setStartStation(startStation);
    }

    //same as above with end station
    std::shared_ptr<Station> endStation;
    endStation = distance->getStationList()->findStation(station2);

    if(endStation != nullptr){
        distance->setEndStation(endStation);
    }


    std::string kmDistance;
    is >> kmDistance;

    //try to convert the string value for distance to an int and set the variable kmDistance if it succeeds
    try {
        int kmDistanceInt = stoi(kmDistance);
        distance->setKmDistance(kmDistanceInt);
    } catch(std::exception &e){
        //otherwise throw the exception again so that it can be caught by the UI class
        throw;
    }

    return is;
}

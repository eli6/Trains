//
// Created by Elin Fritiofsson on 2018-05-28.
//

#ifndef NEWTRAINS_TRAINMAP_H
#define NEWTRAINS_TRAINMAP_H
#include "Station.h"
#include "stationList.h"

class Distance {
private:
    std::shared_ptr<Station> startStation; /**< start Station of distance measurement */
    std::shared_ptr<Station> endStation; /**< end Station of distance measurement */
    int kmDistance; /**< km between the two stations */
    std::shared_ptr<StationList> stationList; /**< list of stations that exist in the program */
public:
    //public members are the basic constructor and getters/setters for the variables:

    Distance(std::shared_ptr<StationList> &newStationList) : stationList(newStationList){};

    std::shared_ptr<StationList> getStationList(){
        return stationList;
    }
    std::shared_ptr<Station> getStartStation() const {
        return startStation;
    }
    std::shared_ptr<Station> getEndStation() const {
        return endStation;
    }
    int getDistance() const {
        return kmDistance;
    }
    void setStartStation(std::shared_ptr<Station> &newStation){
        startStation = newStation;
    }
    void setEndStation(std::shared_ptr<Station> &newStation){
        endStation = newStation;
    }

    void setKmDistance(const int &newDist){
        kmDistance = newDist;
    }

};

/**
 * reads a new distance from the input stream
 * @param is : the input stream
 * @param distance : a new distance to be read in
 * @return a reference to the istream
 */
std::istream &operator>> (std::istream &is, std::unique_ptr<Distance> &distance);

#endif //NEWTRAINS_TRAINMAP_H

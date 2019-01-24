//
// Created by Elin Fritiofsson on 2018-05-26.
//

#include "stationList.h"

std::shared_ptr<Station> StationList::findStation(const std::string &stationName) {

    //creates iterator to a station with a certain name.
    auto it = std::find_if(stations.begin(), stations.end(), [&stationName](const std::shared_ptr<Station> &s){
        return stationName == s->getName();
    });
    if(it!=stations.end()){
        return *it; //return a shared_ptr to the station if one is found.
    } else {
        return nullptr;
    }


}

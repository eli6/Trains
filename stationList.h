//
// Created by Elin Fritiofsson on 2018-05-26.
//

#ifndef NEWTRAINS_STATIONLIST_H
#define NEWTRAINS_STATIONLIST_H

#include "Station.h"
#include "algorithm"
#include "iterator"

class StationList {
public:
    StationList() = default;
    std::vector<std::shared_ptr<Station>> stations; /**< vector of stations in the program */
    std::shared_ptr<Station> findStation(const std::string &stationName); /**< looks for a station with a certain name */
};
#endif //NEWTRAINS_STATIONLIST_H

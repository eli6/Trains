//
// Created by Elin Fritiofsson on 2018-05-22.
//

#include "Vehicle.h"


void Vehicle::logStateChange(const std::string &newState) {
    //add a new state change to the log string
    stateChangeLog+=newState + "\n";

}

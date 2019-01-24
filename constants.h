//
// Created by Elin Fritiofsson on 2018-06-04.
//

#ifndef NEWTRAINS_CONSTANTS_H
#define NEWTRAINS_CONSTANTS_H

/**
 * enum that specifies possible log levels for output
 */
enum logLevel {
    LOW,
    HIGH
};

/**
 * enum for train positions (to check if a train is currently at a its arrival/departure station)
 */
enum trainPositions {
    DEPARTURE_STATION,
    RUNNING_BETWEEN,
    ARRIVAL_STATION
};

#endif //NEWTRAINS_CONSTANTS_H

//
// Created by Elin Fritiofsson on 2018-05-24.
//

#include <iostream>
#include "Train.h"

//array with string versions of train states
const char* StateNames[] = {
        stringify( NOT_ASSEMBLED ),
        stringify( INCOMPLETE ),
        stringify( ASSEMBLED ),
        stringify( READY),
        stringify( RUNNING),
        stringify( ARRIVED),
        stringify( FINISHED),

};

//outputs the strings above for a certain state
std::string Train::outputStateString(State aState) const {
   return StateNames[ aState ];
}


std::istream &operator>>(std::istream &is, std::shared_ptr<Train> &train) {

    std::string trainString;
    getline(is, trainString);
    std::stringstream ss(trainString);

    //types to be assigned to the class from the stringstream
    int id;
    double maxSpeed;
    std::vector<int>vehicletypes;
    int newVehicle;

    std::string idString;
    std::string depStationName;
    std::string arrStationName;
    std::string timeHours;
    std::string timeMinutes;
    std::string maxSpeedString;

    ss >> idString;

    //try to convert.
    try {
        id = std::stoi(idString);
    } catch (std::exception &e) {
        throw;
    }

    ss >> depStationName;
    ss >> arrStationName;


    //find a station with the corresponding name
    std::shared_ptr<Station> depStation = train->getStationList()->findStation(depStationName);
    if(depStation != nullptr){
        //if found : set station as departure station
        train->setDepStation(depStation);
    }
    std::shared_ptr<Station> arrStation = train->getStationList()->findStation(arrStationName);
    if(arrStation != nullptr){
        train->setArrStation(arrStation);
    }


    getline(ss, timeHours, ':'); //t.ex. 01 (:)
    getline(ss, timeMinutes, ' ');


    //remove blank space at the beginning
    timeHours = timeHours.substr(1, timeHours.size());

    //remove the first 0
    if (timeHours[0] == 0) {
        timeHours = timeHours[1];
    }

    if (timeMinutes[0] == 0) {
        timeMinutes = timeMinutes[1];
    }

    //try to convert hours/minutes to int and set values if success.
    try {
        int hours = std::stoi(timeHours);
        int minutes = std::stoi(timeMinutes);
        Time depTime(hours, minutes);
        train->setDepTime(depTime);
        train->setOriginalDepTime(depTime);
        train->setFirstScheduledEventTime(depTime - 30);
    } catch (std::exception &e) {
        throw;
    }

    getline(ss, timeHours, ':'); //t.ex. 01 (:)
    getline(ss, timeMinutes, ' ');


    //remove first zero again.
    if (timeHours[0] == 0) {
        timeHours = timeHours[1];
    }

    if (timeMinutes[0] == 0) {
        timeMinutes = timeMinutes[1];
    }

    try {
        int hours = std::stoi(timeHours);
        int minutes = std::stoi(timeMinutes);
        Time arrTime(hours, minutes);
        train->setArrTime(arrTime);
        train->setOriginalArrTime(arrTime);
    } catch (std::exception &e) {
        throw;
    }

    ss >> maxSpeedString;

    try {
        maxSpeed = std::stod(maxSpeedString);
    } catch (std::exception &e) {
        throw;
    }

    while(ss >> newVehicle){
        vehicletypes.emplace_back(newVehicle);
    }

    train->setVehicleTypes(vehicletypes);
    train->setId(id);

    train->setDelay(0);
    train->setOriginalDelay(0);
    train->setMaxSpeed(maxSpeed);

    //set first state.
    train->setCurrentState(NOT_ASSEMBLED);
    train->originalCarriageOrder = train->getVehicleList();


    return is;
}

void Train::delayDeparture(int delayTime) {
    //delay departure time and arrival time for train
    departureTime = departureTime + delayTime;
    arrivalTime = arrivalTime + delayTime;
    delayed = true;
    //add delay time to train.
    delay.addTime(delayTime);
    //add time to original delay for calculation of departure delays
    originalDelay.addTime(delayTime);
}

void Train::removeDelay() { //as function above but removes the delay
    delayed = false;
    wasDelayed = true;
    originalDelay = delay;
    delay.resetToZero();
    wasDelayed = true;
    arrivalTime = originalArrivalTime;
}


trainPositions Train::getPosition(){
    //all states before 4 mean the train is at the departure station
    if(getState() < 4){
        return DEPARTURE_STATION;
    } else if(getState() > 4) {
        return ARRIVAL_STATION;
    } else {
        return RUNNING_BETWEEN;
    }
}



std::string Train::getData() { return "Train " + std::to_string(id) + " (" +
                               outputState() + ") "
                               + "from " + getDepStation()->getName()
                               + " " + getOriginalDepTime().getTimeString() + " (" + getDepTime().getTimeString() + ")"
                               + " to " + getArrStation()->getName()
                               + " " + getOriginalArrTime().getTimeString() + " (" + getArrTime().getTimeString() + ") "
                               + " departure delay: " + originalDelay.getTimeString() + " arrival delay: " + delay.getTimeString() + " ";

}

void Train::sortCarriages(std::vector<int> &origOrder) {

    std::vector<std::shared_ptr<Vehicle>> tempVector;

    //until we have re-inserted the number of vehicles the train needs
    int counter = 0;
    while (tempVector.size() != originalCarriageOrder.size()) {
    std::vector<int> tempCarriageOrder = originalCarriageOrder;
    for (const auto &vehic : getVehicles()) {
        //if the vehicle has the type that has to be on that position in the train. add it to tempVector.
        if(vehic->getType() == tempCarriageOrder[counter]){
            tempVector.push_back(vehic);
            tempCarriageOrder.erase(tempCarriageOrder.begin()+counter);
        }
    }
        counter++;
    }

    vehicles = tempVector;
}

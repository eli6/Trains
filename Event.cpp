//
// Created by Elin Fritiofsson on 2018-05-30.
//

#include "Event.h"
#include "Simulator.h"
#include <cmath>


Time Event::getEventTime() {
    return eventTime;
}

void AssembleEvent::processEvent(){

    stationHandler->handle(train->getDepStation()); //give the departure station to the station handler
    if(stationHandler->findVehiclesFor(train, eventTime)){ //look for/take the needed vehicles
        train->sortCarriages(train->originalCarriageOrder);
        train->setNewState(ASSEMBLED);
        auto newReadyEvent = std::make_shared<ReadyEvent>(sim, stationHandler, train, eventTime+TIME_TO_READY_EVENT);
        sim->scheduleEvent(newReadyEvent);
    } else { //if at least 1 vehicle could not be found. create a new assembled event:
        auto newAssembleEvent = std::make_shared<AssembleEvent>(sim, train, stationHandler, eventTime+TIME_TO_RETRY_ASSEMBLY);
        sim->scheduleEvent(newAssembleEvent);
        train->setNewState(INCOMPLETE);   //we have one failed attempt so the train state is incomplete
        train->delayDeparture(DELAY_INTERVAL); //add 10 min delay and mark train as delayed
    };
}

void ReadyEvent::processEvent() {
    //this class basically only schedules the running event and doesnt do anything.

    train->setNewState(READY);
    auto newRunningEvent = std::make_shared<RunningEvent>(sim, stationHandler, train, eventTime+MINUTES_TO_RUNNING);
    sim->scheduleEvent(newRunningEvent);

}

void RunningEvent::processEvent() {

    train->setNewState(RUNNING);

    //marks train as having left the station (for statistics purposes)
    train->didLeaveStation(true);

    //calculate distance to trains arrival station (for use in speed calculation last in this function)
    stationHandler->handle(train->getDepStation());
    double distanceInKm = stationHandler->getDistanceTo(train->getArrStation()); //station handler finds distance to arrival station


    if(!train->isDelayed()){ //if train is not delayed this is simple. just schedule next event at the normal arrival time (calculated as minutes ahead).
        auto newArrivedEvent = std::make_shared<ArrivedEvent>(sim, stationHandler, train, eventTime+int(train->getArrTime()-train->getDepTime()));
        sim->scheduleEvent(newArrivedEvent);
        //calculate speed for the trains journey
        double totalArr = train->getArrTime().getTotalMinutes(); //arrival time as minutes from 00:00
        double totalDep = train->getDepTime().getTotalMinutes(); //same with departure
        double travelTimeInHours = (totalArr-totalDep)/60; //travel time in hours calculated
        double kmH = distanceInKm/travelTimeInHours;
        train->setCurrentSpeed(kmH); //speed during the journey is distance(km)/h

    } else {
        double maxSpeed = train->getMaxSpeed(); //191
        double maxSpeedInKmPerMinute = maxSpeed/60;
        double minutesTimeToDistanceAtMaxSpeed = distanceInKm/maxSpeedInKmPerMinute;

        //round to nearest whole number (0.5 rounds upwards)
        int roundedTimeToDistanceAtMaxSpeed = int(floor(minutesTimeToDistanceAtMaxSpeed+0.5));

        auto newArrivedEvent = std::make_shared<ArrivedEvent>(sim, stationHandler, train, eventTime+roundedTimeToDistanceAtMaxSpeed);

        //check that the train will not arrive to early now, in that case: set arrival time to standard arrival time.
        if(newArrivedEvent->getEventTime().getTotalMinutes() <= train->getOriginalArrTime().getTotalMinutes()) {
            train->removeDelay();
            newArrivedEvent->setEventTime(train->getOriginalArrTime().getTotalMinutes());
        } else {
            train->setArrTime(newArrivedEvent->getEventTime().getTotalMinutes());
            train->setDelay(newArrivedEvent->getEventTime().getTotalMinutes() - train->getOriginalArrTime().getTotalMinutes());
        };

        double travelTimeInHours = (minutesTimeToDistanceAtMaxSpeed)/60; //travel time in hours calculated
        double kmH = distanceInKm/travelTimeInHours;
        train->setCurrentSpeed(kmH); //speed during the journey is distance(km)/h
        sim->scheduleEvent(newArrivedEvent);
    }


}

void ArrivedEvent::processEvent() {
    //also a simple class. just schedules a new finished event after the specified interval "MINUTES_TO_FINISHED"
    train->setNewState(ARRIVED);
    auto newFinishedEvent = std::make_shared<FinishedEvent>(sim, stationHandler, train, eventTime+MINUTES_TO_FINISHED);
    sim->scheduleEvent(newFinishedEvent);

}




void Event::logEvent(){
    std::string fileName = "TrainSim.log";
    std::ofstream myFile;
    myFile.open(fileName, std::ios::app);
    //write log message for an event to file.
    myFile << getLogMessage() << std::endl;
}


std::string Event::getLogMessage(){
    //composition of log message with relevant information
    std::string logMessage = eventTime.getTimeString() + " "
                             + "Train " + std::to_string(train->getId()) + " (" +
                             train->outputState() + ") "
                             + "from " + train->getDepStation()->getName()
                             + " " + train->getOriginalDepTime().getTimeString() + " (" + train->getDepTime().getTimeString() + ")"
                             + " to " + train->getArrStation()->getName()
                             + " " + train->getOriginalArrTime().getTimeString() + " (" + train->getArrTime().getTimeString() + ") "
                             + " delay: (" + train->getDelay().getTimeString() + ") " +
                             " Speed: " + std::to_string(train->getCurrentSpeed());
    return logMessage;

}


std::string Event::getShortLogMessage(){
    //shorter version of log message for onscreen with log level LOW
    std::string logMessage = eventTime.getTimeString() + " "
                             + "Train " + std::to_string(train->getId()) + " (" +
                             train->outputState() + ") ";
    return logMessage;

}


void FinishedEvent::processEvent() {
    train->setNewState(FINISHED);
    //ask station handler to return the carriages to the station for the train
    stationHandler->handle(train->getArrStation());
    stationHandler->returnCarriagesFromTrain(train, eventTime);

}
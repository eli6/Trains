//
// Created by Elin Fritiofsson on 2018-06-01.
//

#include "Simulator.h"

int Simulator::nextLimit(){
    /*if the last handled time + interval length exceeds the end time of simulation
       return the end time. else just return the last handled time + interval length.
     */
    return chosen_end_time.getTotalMinutes() < lastTimeLimit+intervalLength ? chosen_end_time.getTotalMinutes() : lastTimeLimit+intervalLength;
}


void Simulator::simulate(){
    finishedOneRun = false; //variable to control if simulation is to run repeatedly.
    if(eventClassQueue.empty()){
        simulationReachedEnd = true; //if queue is empty -> simulation must end
    }
    //while the next event's time is smaller than or equal to the nextLimit() and finishedOneRun is false.
    while(int nextEventTime = eventClassQueue.top()->getEventTime().getTotalMinutes() <= nextLimit() && !finishedOneRun){
        if(!repeatedSimulation){
            finishedOneRun = true;
        }
        //deal with the next event.
        handleEvent();
    }
    //after loop, add the handled interval length to the last time handled.
    lastTimeLimit += intervalLength;
}

void Simulator::nextEvent(){
    //set start time to time for the next event.
    chosen_start_time.setTime(eventClassQueue.top()->getEventTime().getTotalMinutes());

    //save the original values so they can be restored
    int tempInterval = intervalLength;
    Time tempEndTime = chosen_end_time;

    //also set the end time to the time for the next event
    chosen_end_time = eventClassQueue.top()->getEventTime();

    //set interval up until our event time.
    intervalLength = chosen_end_time.getTotalMinutes() - lastTimeHandled.getTotalMinutes();

    //this will only run once so repeated simulation is false.
    repeatedSimulation = false;

    //if our event time is later than the original (definitive) end time of simulation
    if(chosen_end_time.getTotalMinutes() >= tempEndTime.getTotalMinutes()){
        //just finish up running/arrived trains and show all those instead of next event.
        finishUp();
    } else {
        //do normal simulation of the next event.
        simulate();
    }

    //restore repeatedSimulation to true.
    repeatedSimulation = true;

    //restore original interval and end time values.
    intervalLength = tempInterval;
    chosen_end_time = tempEndTime;
}

void Simulator::finishUp() {
    printStarLine();
    std::cout << "SIMULATION END TIME REACHED. FINISHING TRAINS THAT ARE ON THEIR WAY:" << std::endl;
    printStarLine();
    while(!eventClassQueue.empty()){
        //if we have a train state that is not ready or running
        if(eventClassQueue.top()->getTrain()->getState() != RUNNING &&
           eventClassQueue.top()->getTrain()->getState() != ARRIVED){

            //just pop the event and do nothing if the queue isn't empty.
            if(!eventClassQueue.empty()) {
                eventClassQueue.pop();
            }
        } else {
            //only set last handled time if we actually handle the event
            lastTimeLimit = eventClassQueue.top()->getEventTime().getTotalMinutes();
            handleEvent();


        }
    }
    simulationReachedEnd = true;


}

void Simulator::handleEvent(){
    std::shared_ptr<Event> nextEvent = eventClassQueue.top();
    nextEvent->processEvent();

    //if we have passed the start time: print the result on screen.
    if(nextEvent->getEventTime().getTotalMinutes() >= chosen_start_time.getTotalMinutes()){

        //print long or short message depending on log level.
        logLevel level = currentLogLevel;
        if(level == LOW){
            std::cout << nextEvent->getShortLogMessage() << std::endl;
        } else {
            std::cout << nextEvent->getLogMessage() << std::endl;

        }

    }

    nextEvent->addToTrainHistory();
    nextEvent->logEvent(); //log to file.

    eventClassQueue.pop();
}


void Simulator::run(){
    //set interval so that it reaches the simulation end time.
    intervalLength = chosen_end_time.getTotalMinutes() - lastTimeHandled.getTotalMinutes();
    simulate();
    //if the queue is not empty by then. run the finishUp() function.
    if(!eventClassQueue.empty()){
        finishUp();
    }
    returnToMenu();
}

void Simulator::runNextInterval() {

        //set the start time to previously handled time.
        chosen_start_time.setTime(lastTimeLimit);

        //if running the interval means we reaches the end time, finish up as well.
        if(lastTimeHandled+intervalLength >= chosen_end_time.getTotalMinutes()){
            simulate();
            finishUp();
        } else {
            simulate();
        }

}

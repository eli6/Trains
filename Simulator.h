//
// Created by Elin Fritiofsson on 2018-06-01.
//

#ifndef NEWTRAINS_SIMULATORREAL_H
#define NEWTRAINS_SIMULATORREAL_H


#include "Event.h"
#include "constants.h"
#include "FunctionLibrary/functionLib.h"

class Simulator {
private:
    /**
     * comparison class for the priority queue. event with next event time first.
     */
    class EventComparison {
    public:
        bool operator() (std::shared_ptr<Event> &left, std::shared_ptr<Event> &right) {
            return left->getEventTime() > right->getEventTime();
        }
    };
    /**< priority queue for the events */
    std::priority_queue<std::shared_ptr<Event>, std::vector<std::shared_ptr<Event>>, EventComparison> eventClassQueue;

    Time chosen_end_time;
    Time chosen_start_time;
    int intervalLength;
    int lastTimeLimit;
    Time lastTimeHandled;
    bool repeatedSimulation;
    bool finishedOneRun;
    bool simulationReachedEnd; /**< so program knows when to display the simulation-ended-menu. */
    logLevel currentLogLevel;

    int nextLimit(); /**< next limit for the simulation (where it should stop) */
    void simulate(); /**< simulates either until end of interval, once(if repeatedSimulation=true) or until the end time for simulation*/
    void finishUp();
    void handleEvent();
public:
    //GETTERS/SETTERS:
    bool simFinished(){ return simulationReachedEnd; }
    int getInterval(){ return intervalLength; }
    Time getLastTimeHandled(){
        lastTimeHandled.setTime(lastTimeLimit);
        return lastTimeHandled;
    }
    void setIntervalLength(const int &newInterval){
        intervalLength = newInterval;
    }
    std::string getChosenStart(){
        return chosen_start_time.getTimeString();
    }
    std::string getChosenEnd(){
        return chosen_end_time.getTimeString();
    }

    void setStartTime(const Time &newStart){
        chosen_start_time = newStart;
    }
    void setEndTime(const Time &newEnd){
        chosen_end_time = newEnd;
    }
    //END OF BASIC GETTERS/SETTERS.


    Simulator(){
        chosen_end_time.setTime(1439); /**< sets default end time to 23:59 */
        chosen_start_time.setTime(0); /**< default start to 00:00 */
        intervalLength = 10;
        lastTimeLimit = 0;
        repeatedSimulation = true;
        currentLogLevel = HIGH;
        simulationReachedEnd = false;
        finishedOneRun = false;
    }

    //PUBLIC FUNCTIONS:
    void scheduleEvent(const std::shared_ptr<Event> &event){
        //adds event to priority queue
        eventClassQueue.push(event);
    }
    void run(); /**< runs simulation until the end */
    void runNextInterval(); /**< runs simulation until end of next interval */
    void nextEvent(); /**< runs only next event */

};


#endif //NEWTRAINS_SIMULATORREAL_H

//
// Created by Elin Fritiofsson on 2018-05-30.
//

#ifndef NEWTRAINS_EVENT_H
#define NEWTRAINS_EVENT_H
#include "Train.h"
#include "StationHandler.h"
#include "Time.h"
#include <iomanip>
#include <fstream>

class Simulator;
class Event {
protected:
    std::shared_ptr<Train> train; /**< the train that is currently participating in the event */
    Time eventTime; /**< time this event is taking place */
    std::shared_ptr<StationHandler> stationHandler; /**< station handler to look for wagons at stations for the assembly */
    std::shared_ptr<Simulator> sim; /**< simulator (event will be returned to its priority queue) */
public:
    virtual ~Event() = default; /**< virtual destructor */

    Event() = default;
    Event(std::shared_ptr<Train> &newTrain, const Time &newEventTime, std::shared_ptr<Simulator> &newSim, std::shared_ptr<StationHandler> &newStationHandler) : train(newTrain), eventTime(newEventTime), sim(newSim), stationHandler(newStationHandler){};
    virtual void processEvent() = 0; /**<virtual processEvent function for this base class */
    void logEvent(); /**< writes the event information to the log file */
    std::string getShortLogMessage(); /**< returns a short log message for printing on screen with log level LOW */
    void addToTrainHistory(){ /**< adds this event information to the trains history */
        train->addToMessageHistory(getLogMessage());
    };
    std::string getLogMessage(); /**< gets a normal-size log message for this event (used by logEvent() and for printing onscreen) */

    //needed setters and getters here below:

    Time getEventTime();
    void setEventTime(const Time &newTime){
        eventTime = newTime;
    }
    std::shared_ptr<Train> getTrain() {
        return train;
    };


};

class AssembleEvent : public Event {
private:
    int DELAY_INTERVAL = 10; /**< departure delay if assembly fails */
    int TIME_TO_READY_EVENT = 20; /**< time until next event (= ready) */
    int TIME_TO_RETRY_ASSEMBLY = 10; /**< time until assembly should be tried again after failing */
public:
    AssembleEvent(std::shared_ptr<Simulator> &newSim, std::shared_ptr<Train> &train, std::shared_ptr<StationHandler> &newStationHandler, const Time &newEventTime) : Event(train, newEventTime, newSim, newStationHandler){};
    void processEvent() override; /**< override of processEvent for this child class*/

};

class ReadyEvent : public Event {
private:
    int MINUTES_TO_RUNNING = 10; /**< time until next event (= running) */
public:
    ReadyEvent(std::shared_ptr<Simulator> newSim, std::shared_ptr<StationHandler> &newStationHandler, std::shared_ptr<Train> &train, const Time &newEventTime) : Event(train, newEventTime, newSim, newStationHandler){};
    void processEvent() override; /**< override of processEvent for this child class*/
};

class RunningEvent : public Event {
public:
    RunningEvent(std::shared_ptr<Simulator> newSim, std::shared_ptr<StationHandler> &newStationHandler,std::shared_ptr<Train> &train, const Time &newEventTime) : Event(train, newEventTime, newSim, newStationHandler){};
    void processEvent() override; /**< override of processEvent for this child class*/
};

class ArrivedEvent : public Event {
private:
    int MINUTES_TO_FINISHED = 20; /**< time until next event (= finished) */
public:
    ArrivedEvent(std::shared_ptr<Simulator> &newSim, std::shared_ptr<StationHandler> &newStationHandler, std::shared_ptr<Train> &train, const Time &newEventTime) : Event(train, newEventTime, newSim, newStationHandler){};
    void processEvent() override; /**< override of processEvent for this child class*/
};

class FinishedEvent : public Event {
public:
    FinishedEvent(std::shared_ptr<Simulator> &newSim, std::shared_ptr<StationHandler> &newStationHandler, std::shared_ptr<Train> &train, const Time &newEventTime) : Event(train, newEventTime, newSim, newStationHandler){};
    void processEvent() override; /**< override of processEvent for this child class*/
};

#endif //NEWTRAINS_EVENT_H

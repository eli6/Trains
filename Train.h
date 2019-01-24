//
// Created by Elin Fritiofsson on 2018-05-24.
//

#ifndef UNTITLED1_TRAIN_H
#define UNTITLED1_TRAIN_H
#define stringify(StateNames) # StateNames
#include <string>
#include <vector>
#include <sstream>
#include "Station.h"
#include "Time.h"
#include "stationList.h"
#include "constants.h"

/**
 * POSSIBLE TRAIN STATES
 */
enum State {
    NOT_ASSEMBLED,
    INCOMPLETE,
    ASSEMBLED,
    READY,
    RUNNING,
    ARRIVED,
    FINISHED
};

class Train {
private:
    bool leftStation; /**< if train has left station: useful for calculating delay only for trains that left station */
    int id;
    Time departureTime;/**< actual departure time (can be delayed) */
    Time arrivalTime;/**< actual arrival time (can be delayed) */
    std::shared_ptr<Station> depStation;
    std::shared_ptr<Station> arrStation;
    State currentState; /**< current train state */
    bool delayed; /**< is train delayed or not? */
    Time originalDepartureTime; /**< departure time according to timetable */
    Time originalArrivalTime; /**< arrival time according to timetable */
    Time delay = 0;
    double currentSpeed; /**< current speed (if train has been in state running the speed is stored here)*/
    Time firstScheduledEventTime; /**< first time an event is to take place (normally an assembly event: 30 min before departure) */
    std::vector<std::string> eventHistory; /**< event history for logging and display purposes */
    double maxSpeed; /**< max speed of train (for calculations)*/
    std::vector<int>vehicleComposition; /**< the vehicle types the train (still) needs*/
    std::vector<std::shared_ptr<Vehicle>> vehicles; /**< vehicles the train has found*/
    std::shared_ptr<StationList> stationList; /**< a list of stations for the istream reading */
    bool wasDelayed = false;
    Time originalDelay = 0; /**< original delay. can differ from "delay" if the train can catch up while running. */
public:
    std::vector<int>originalCarriageOrder; /**< the vehicle types the train needs*/

    Train(std::shared_ptr<StationList> &newStationList) : stationList(newStationList) {
        currentState = NOT_ASSEMBLED;
        firstScheduledEventTime = departureTime;
        delayed = false;
        currentSpeed = 0;
        id = 0;
        maxSpeed = 0;
        leftStation = false;
    }

    void delayDeparture(int delayTime); /**<adds delayTime and sets necessary bool values for a delayed train */
    trainPositions getPosition(); /**< get position to see if the train is at a certain station or not */
    void removeDelay(); /**< removes delay and sets the train as not delayed (train is still marked as "was delayed") */
    void sortCarriages(std::vector<int> &origOrder);
    //----------------
    // BASIC SETTERS/GETTERS
    //-----------------
    bool hasLeftStation(){
        return leftStation;
    }
    void didLeaveStation(bool value){
        leftStation = value;
    }
    void setCurrentSpeed (const double &newSpeed){
        currentSpeed = newSpeed;
    }
    void setId(const int newId) { id = newId; };
    void setDepTime(Time newTime) { departureTime = newTime; };
    void setArrTime(Time newTime) { arrivalTime = newTime; };
    void setOriginalArrTime(Time newTime) { originalArrivalTime = newTime; };
    void setOriginalDepTime(Time newTime) { originalDepartureTime = newTime; };
    void setDelay( int newDelayMinutes ) { delay.setTime(newDelayMinutes); };
    void setOriginalDelay( int newDelayMinutes ) { originalDelay.setTime(newDelayMinutes);};

    void setDepStation(const std::shared_ptr<Station> &newDepStation) { depStation = newDepStation; };
    void setArrStation(const std::shared_ptr<Station> &newArrStation) { arrStation = newArrStation; };
    void setMaxSpeed(const double newMax) { maxSpeed = newMax; };
    void setCurrentState(State newState){ currentState = newState; };
    void setFirstScheduledEventTime(Time newTime) { firstScheduledEventTime = newTime; };
    void setNewState(State newState){ currentState = newState; };
    void setVehicleTypes(std::vector<int> newVehicleTypes) {
        vehicleComposition = newVehicleTypes;
    };
    std::shared_ptr<StationList> const getStationList(){ return stationList; }
    State getState() const { return currentState; };
    std::string outputState() const { return outputStateString(currentState ); };
    double getCurrentSpeed(){ return currentSpeed; }
    std::vector<std::string> getMessageHistory(){ return eventHistory; }
    Time getOriginalArrTime() const { return originalArrivalTime; };
    Time getDelay() const { return delay; };
    Time getOriginalDelay() const { return originalDelay; };

    Time getDepTime() const { return departureTime; };
    Time getOriginalDepTime() const { return originalDepartureTime; };
    Time getArrTime() const { return arrivalTime; };
    double getMaxSpeed() const { return maxSpeed; };
    std::shared_ptr<Station> getDepStation() const { return depStation; };
    std::shared_ptr<Station> getArrStation() const { return arrStation; };
    std::vector<int> const getVehicleList() {
        return vehicleComposition;
    }
    std::vector<std::shared_ptr<Vehicle>> getVehicles(){
        return vehicles;
    }
    int getId() const { return id; };
    bool isDelayed() const { return delayed; }
    bool trainWasDelayed() const { return wasDelayed; };
    Time getFirstScheduledEventTime() const { return firstScheduledEventTime; };
    //----------------
    // END OF SETTERS/GETTERS
    //-----------------

    void addVehicle(std::shared_ptr<Vehicle> &newVehic){ vehicles.emplace_back(newVehic); } /**< adds vehicle when a train has found it */

    void addToMessageHistory(const std::string &newMessage) { eventHistory.push_back(newMessage); } /**< adds new log message to train history */
    void removeFromVehicleList(int type){ /**< removes the vehicle type from the vehicle list after it was found */
        auto it = std::find_if(vehicleComposition.begin(), vehicleComposition.end(), [&type](const int &t){
            return t == type;
        });
        vehicleComposition.erase(it);
    }

    std::string getData(); /**< returns train data for display */
    std::string outputStateString( State aState ) const; /**< outputs the train state as a string */
};

/**
 * reads train from input stream
 * @param is : input stream
 * @param train  : train to be read
 * @return : the input stream
 */
std::istream &operator>> (std::istream &is, std::shared_ptr<Train> &train);


#endif //UNTITLED1_TRAIN_H

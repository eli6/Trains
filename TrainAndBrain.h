//
// Created by Elin Fritiofsson on 2018-05-23.
//

#ifndef UNTITLED1_Simulator_H
#define UNTITLED1_Simulator_H


#include "Station.h"
#include "Train.h"
#include "stationList.h"
#include "Distance.h"
#include "Event.h"
#include "Simulator.h"
#include <fstream>
#include <queue>
#include <iostream>
#include <iomanip>
#include "Menu.h"
#include "FunctionLibrary/functionLib.h"

class TrainAndBrain {
private:
    /**< menu variables for the different menus */
    Menu mainMenu;
    Menu firstMenu;
    Menu trainMenu;
    Menu stationMenu;
    Menu vehicleMenu;
    Menu finishedMenu;
    Menu stationSubmenu;
    Menu statsMenu;
    logLevel currentLogLevel; /**< global log level for screen display */
    static const char* logLevelNames[]; /**< string conversion for log levels */
    std::shared_ptr<StationHandler> stationHandler;
    std::shared_ptr<Simulator> theSim;
    std::shared_ptr<StationList> stationList;
    std::vector<std::shared_ptr<Train>> trains;
    void changeLogLevel();
    void printVehicleInfo(const std::shared_ptr<Vehicle> &vehic); /**< prints information for one vehicle */

    //trainMenuFuncs
    void searchTrainByNumber();
    void searchTrainByVehicleId();
    void showAllTrains();

    //stationMenu funcs
    void runStationMenu(); /**< displays the station menu */
    void showStationNames();
    void showStationByName();
    void showAllStations();
    void runStationSubmenu(const std::string &stationName); /**< submenu to choose information for a particular station */
    void showStationTrains(std::shared_ptr<Station> &station);
    void showStationVehicles(std::shared_ptr<Station> &station);

    //vehicleMenu funcs
    void runVehicleMenu(); /**< displays the vehicle menu y */
    void showVehicleById();
    void showAllVehicles();
    void showSimHistoryForVehicle(); /**< simulation history for a vehicle */

    void showTimetable(); /**< timetable with original departure/arrival times for all trains */

    void changeStartTime(); /**< start time for simulation */
    void changeEndTime(); /**< end time for simulation */
    void changeIntervalLength(); /**< change simulation interval length */
    void scheduleTrains();
    void printTrainLogForTrain(int id); /**< train log for train with a specific id */

    Time getArrDelay();
    Time getDepDelay();
    void delayedTrains(); /**< prints all delayed trains (that are still delayed at the end of simulation) */
    void printTrainInfo(const std::shared_ptr<Train> &train);
    void printFullInfoForTrain(int id);
    void printTrainsThatWereOnTime();
    void printTrainsThatNeverLeftTheStation();
public:
    TrainAndBrain(){
        theSim = std::make_unique<Simulator>();
        stationList = std::make_shared<StationList>();
        stationHandler = std::make_shared<StationHandler>(stationList);

        // setting up the different menus (below)
        mainMenu.setItems({"Change interval length [" + std::to_string(theSim->getInterval()) + "]",
                           "Run next interval",
                           "Next event",
                           "Finish (complete simulation)",
                           "Change log level",
                           "Train menu",
                           "Station menu",
                           "Vehicle menu",
                           "Show timetable",
                           "Quit"});

        firstMenu.setTitle("START MENU");
        firstMenu.setItems({"Change start time [" + theSim->getChosenStart() + "]",
                            "Change end time [" + theSim->getChosenEnd() + "]",
                            "Start Simulation",
                            "Quit"});

        trainMenu.setTitle("TRAIN MENU");
        trainMenu.setItems({"Search train by number",
                            "Search train by vehicle id",
                            "Show all trains",
                            "Show simulation history for train",
                            "Change log level",
                           "Return"});

        stationMenu.setTitle("STATION MENU");
        stationMenu.setItems({"Show station names",
                            "Show station by name",
                            "Show all stations",
                            "Change log level",
                            "Return"});

        stationSubmenu.setTitle("MENU FOR STATION");
        stationSubmenu.setItems({"Show trains at station",
                              "Show vehicles at station",
                              "Change log level",
                              "Return"});

        vehicleMenu.setTitle("VEHICLE MENU");
        vehicleMenu.setItems({"Show vehicle (information & current position) by id",
                              "Show all vehicles",
                              "Show sim history for vehicle",
                              "Change log level",
                              "Return"});

        finishedMenu.setTitle("SIMULATION MENU [FINISHED]");
        finishedMenu.setItems({"Change log level",
                              "Print statistics",
                              "Train menu",
                              "Station menu",
                              "Vehicle menu",
                              "Return"});

        statsMenu.setTitle("STATISTICS");
        statsMenu.setItems({"Print total delay times",
                               "Trains that never left the station",
                               "Trains that were on time",
                               "Delayed trains",
                               "Return"});

        //default log level is high.
        currentLogLevel = HIGH;
    }

    void readStationsFromFile(); /**< reading stations and vehicles there */
    void readTrainMapFromFile(); /**< reading train map with distances between stations */
    void readTrainsFromFile(); /**< reading trains and their wagon information */
    void runFinishedMenu(bool &again); /**< menu displayed when simulation reaches end */
    void runSimulationMenu();
    void showSimHistoryForTrain();
    void runTrainMenu();
    void printTotalDelays(); /**< prints total departure and arrival delay */
    void printStatistics(); /**< prints the statistics menu */
    void run();
};


#endif //UNTITLED1_Simulator_H

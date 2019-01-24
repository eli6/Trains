//
// Created by Elin Fritiofsson on 2018-05-23.
//
#include "TrainAndBrain.h"

//timetable comparision function : first departure first.
class TimetableCompare
{
public:
    bool operator() (std::shared_ptr<Train> &left, std::shared_ptr<Train> &right)
    {
        return left->getOriginalDepTime() > right->getOriginalDepTime();
    }
};

//adds a newline to the end of a file (on mac osx the last line was skipped if there was no newline).
void outputNewlineTo(const std::string &fileName){
	
    std::fstream outFile;
    outFile.open(fileName, std::ios::app);
    outFile << '\r';
	outFile << '\n';
    outFile.close();
	
}

//removes the added newline from the file
void removeNewlineFrom(const std::string &fileName){
	
    std::fstream outfile;
    outfile.open(fileName, std::ios::in|std::ios::out);
	outfile.seekp(-1, std::ios::end);
	outfile.write(" ", 1);
    outfile.close();

}

//string versions of log levels.
const char* TrainAndBrain::logLevelNames[] = {
        stringify( LOW ),
        stringify( HIGH ),

};


void TrainAndBrain::readStationsFromFile() {

	std::stringstream ss;
    std::string fileName = "TrainStations.txt";
    //first adds a new line to the file at the end
    outputNewlineTo(fileName);

    std::ifstream inFile(fileName);

    //create bitmask
    std::ios_base::iostate mask = std::ios::eofbit | std::ios::failbit | std::ios::badbit;

    //setting the above mask for the stream.
    inFile.exceptions(mask);

    if (inFile) {
        bool finished = false;
        while (!finished) {
            auto newStation = std::make_unique<Station>();
            // >> might throw an exception so we add a "try".
            try {
                try {
                        //read in a new Station from file
                        inFile >> newStation;
                        //put it in the station list
                        stationList->stations.emplace_back(std::move(newStation));

                } catch (std::ios_base::failure &) {

                    std::ios_base::iostate flag = inFile.rdstate();

                    if (flag & std::ios::eofbit) {
                        finished = true;
                    } else {
                        //print error if we have failbit/badbit
                        std::string error;
                        getline(inFile, error);

                        std::cout << error << std::endl;
                    }
                }
                } catch(std::exception &e){
                    //error message if file could not be read
                    throw(std::runtime_error("Could not read file " + fileName));
                }
        }
        inFile.close();//need to add an else or sth... or if!isOpen.
        removeNewlineFrom(fileName);
    }
}


void TrainAndBrain::readTrainsFromFile() {

    std::string fileName = "Trains.txt";
    outputNewlineTo(fileName);
    std::ifstream inFile(fileName);

    std::ios_base::iostate mask = std::ios::eofbit | std::ios::failbit | std::ios::badbit;

    //setting the above mask for the stream.
    inFile.exceptions(mask);

    if (inFile) {
        bool finished = false;

        // >> might throw an exception so we add a "try".
        while (!finished) {
            auto newTrain = std::make_shared<Train>(stationList);
            try{
                try {
                    //read in a new train from file

                        inFile >> newTrain;
                        trains.push_back(newTrain);


                } catch (std::ios_base::failure &) {

                    std::ios_base::iostate flag = inFile.rdstate();

                    if (flag & std::ios::eofbit) {
                        finished = true;
                    } else {
                        //print error if we have failbit/badbit
                        std::string error;
                        getline(inFile, error);

                        std::cout << error << std::endl;
                    }
                }
            } catch(std::exception &e) {
                throw std::runtime_error("Could not read file " + fileName);
            }
        }
        inFile.close();
        removeNewlineFrom(fileName);
    }
}







void TrainAndBrain::readTrainMapFromFile() {
    std::string fileName = "TrainMap.txt";
    outputNewlineTo(fileName);
    std::fstream inFile(fileName);

    std::ios_base::iostate mask = std::ios::eofbit | std::ios::failbit | std::ios::badbit;

    //setting the above mask for the stream.
    inFile.exceptions(mask);

    if (inFile) {
        bool finished = false;

        // >> might throw an exception so we add a "try".
        while (!finished) {
            auto newDistance = std::make_unique<Distance>(stationList);
            try {
                try {
                    //read in a new distance from file
                    inFile >> newDistance;
                    stationHandler->distances.emplace_back(*newDistance);
                } catch (std::ios_base::failure &) {

                    std::ios_base::iostate flag = inFile.rdstate();

                    if (flag & std::ios::eofbit) {
                        finished = true;
                    } else {
                        //print error if we have failbit/badbit
                        std::string error;
                        getline(inFile, error);

                        std::cout << error << std::endl;
                    }
                }
            } catch(std::exception &e) {
                throw std::runtime_error("Could not read file " + fileName);
            }
        }
        inFile.close();
        removeNewlineFrom(fileName);
    }
}


void TrainAndBrain::changeStartTime() {
    Time startTime;
    inputTime(startTime);
    theSim->setStartTime(startTime);
}

void TrainAndBrain::changeEndTime() {
    Time endTime;
    inputTime(endTime);
    theSim->setEndTime(endTime);

}


void TrainAndBrain::searchTrainByNumber() {
    int trainId;
    std::cout << "Please enter train id" << std::endl;
    inputAndCheckIfInt(trainId);
    printFullInfoForTrain(trainId);

}

void TrainAndBrain::searchTrainByVehicleId() {
    int vehicleId;
    std::cout << "Please enter vehicle id" << std::endl;
    inputAndCheckIfInt(vehicleId);
    for(const auto &train : trains){
        for(const auto &v : train->getVehicles()) {
            if(v->getId() == vehicleId){
                printTrainInfo(train);
            }
        }
    }
}

void TrainAndBrain::showAllTrains() {
    //prints info for each train:
    std::for_each(trains.begin(), trains.end(), [this](const std::shared_ptr<Train> &train){
        this->printTrainInfo(train);
    });

}


void TrainAndBrain::runStationMenu() {
    bool secondAgain = true;

    do {

        //updates the log level display before menu is shown.
        stationMenu.setItem(3 , "Change log level [" + std::string(logLevelNames[currentLogLevel]) + "]");


        stationMenu.printMenuItems();

        switch (stationMenu.getMenuChoice()) {
            case 1: showStationNames();
                break;
            case 2: showStationByName();
                break;
            case 3: showAllStations();
                break;
            case 4: changeLogLevel();
                break;
            case 5: secondAgain = false;
                break;
            default:
                break;
        }
    } while (secondAgain);

}

void TrainAndBrain::showSimHistoryForVehicle(){
    std::cout << "Enter vehicle id: " << std::endl;
    int id;
	bool found = false;
    //validate the input
    inputAndCheckIfInt(id);
    for(const auto &station : stationList->stations){
        for(const auto &vehicle : station->getVehicles()){
            //check vehicles at the stations till we find our vehicle
            if(vehicle->getId() == id){
                //then print its info and event history
				found = true;
                printVehicleInfo(vehicle);
                printLine();
                std::cout << "HISTORY FOR VEHICLE" << std::endl;
                printLine();
                std::cout << vehicle->getStateChangeHistory() << std::endl;
            }

        };
    }
	if (!found) {
		for (const auto &train : trains) {
			for (const auto &v : train->getVehicles()) {
				if (v->getId() == id && train->getState() != FINISHED) {
					printVehicleInfo(v);
					printLine();
					std::cout << "HISTORY FOR VEHICLE" << std::endl;
					printLine();
					std::cout << v->getStateChangeHistory() << std::endl;


				}
			}
		}
	}
	returnToMenu();
}

void TrainAndBrain::runVehicleMenu() {
    bool secondAgain = true;

    do {
        //update log level display
        vehicleMenu.setItem(3 , "Change log level [" + std::string(logLevelNames[currentLogLevel]) + "]");

        vehicleMenu.printMenuItems();

        switch (vehicleMenu.getMenuChoice()) {
            case 1: showVehicleById();
                break;
            case 2: showAllVehicles();
                break;
            case 3: showSimHistoryForVehicle();
                break;
            case 4: changeLogLevel();
                break;
            case 5: secondAgain = false;
                break;
            default:
                break;
        }
    } while (secondAgain);

}

void TrainAndBrain::showStationNames() {
    //show each station's name:
    std::for_each(stationList->stations.begin(), stationList->stations.end(), [](std::shared_ptr<Station> &station){
        std::cout << station->getName() << std::endl;
    });
}

void TrainAndBrain::printVehicleInfo(const std::shared_ptr<Vehicle> &vehic){
    //get the vehicle info (vector of pairs)
    std::vector<std::pair<std::string, std::string>> mappy = vehic->getInfo();

    //print everything if log level is HIGH
    if(currentLogLevel == HIGH){
        for(const auto &info : mappy){
            std::cout << info.first << ": " << info.second << "|";
        }
        std::cout << std::endl;
    } else { //print only first two pairs if log level isn't HIGH.
        std::for_each(mappy.begin(), mappy.begin() + 2, [](const std::pair<std::string, std::string> &info) {
            std::cout << info.first << ": " << info.second << "|";
        });
        std::cout << std::endl;
    }
}

void TrainAndBrain::showStationTrains(std::shared_ptr<Station> &aStation){
    printStarLine();
    std::cout << "***   Welcome to " << aStation->getName() << "   ***" << std::endl;
    printStarLine();
    std::cout << "Trains currently at this station: " << std::endl;
    printLine();
    for(const auto &t : trains){
        trainPositions pos = t->getPosition();
        //if the train is at departure station:
        if(pos == DEPARTURE_STATION){
            //check if departure station is this station. in that case: print the train info here.
            if(t->getDepStation() == aStation){
                printTrainInfo(t);
            }
        } else if(pos == ARRIVAL_STATION){ //if the train is at arrival station:
            //check if arrival station is this station. in that case: print the train info here.
            if(t->getArrStation() == aStation){
                printTrainInfo(t);
            }
        }
    }
}

void TrainAndBrain::showStationVehicles(std::shared_ptr<Station> &aStation){
    for(const auto &v : aStation->getVehicles()){
        printVehicleInfo(v);
    }
}

void TrainAndBrain::showAllStations() {
    for(const auto &station : stationList->stations){
        printStarLine();
        std::cout << station->getName() << std::endl;
        printStarLine();
        std::cout << "Trains currently at this station: " << std::endl;
        for(const auto &t : trains){
            trainPositions pos = t->getPosition();
            if(pos == DEPARTURE_STATION){ //if a train is at departure station and it is this station:
                if(t->getDepStation() == station){
                    //print train id for the train
                    std::cout << t->getId() << " | ";
                }
            } else if(pos == ARRIVAL_STATION){ //same as for departure station above
                if(t->getArrStation() == station){
                    std::cout << t->getId()  << " | ";
                }
            }
        }
        std::cout << std::endl;
        printLine();
        //print number of vehicles at the station:
        std::cout << station->getVehicles().size() << " vehicles are currently at this station." << std::endl;
        std::cout << "(search for station individually for more information.)" << std::endl;
        printLine();
    }

}

void TrainAndBrain::runStationSubmenu(const std::string &stationName){

    std::shared_ptr<Station> aStation = stationList->findStation(stationName);
    //if stationName can be found in the station list:
    if(aStation!= nullptr){
        bool again = true;
        do {
            //set station name as menu title
            stationSubmenu.setTitle("Menu for station " + stationName);
            //update log level display
            stationSubmenu.setItem(2 , "Change log level [" + std::string(logLevelNames[currentLogLevel]) + "]");

            stationSubmenu.printMenuItems();

            switch (finishedMenu.getMenuChoice()) {
                case 1: showStationTrains(aStation);
                    break;
                case 2: showStationVehicles(aStation);
                    break;
                case 3: changeLogLevel();
                    break;
                case 4: again = false;
                    break;
                default:
                    break;
            }
        } while (again);
    } else {
        std::cout << "Station not found" << std::endl;
    }

}


void TrainAndBrain::showStationByName() {
    std::string name;
    std::cout << "Please enter station name " << std::endl;
    getline(std::cin, name);
    //run the submeny for that station (if it's not found the submenu will display a corresponding message instead)
    runStationSubmenu(name);
}

void TrainAndBrain::showVehicleById() {
    int id;
    std::cout << "Please enter vehicle id " << std::endl;
    inputAndCheckIfInt(id);
    bool found = false;
    for(const auto &station : stationList->stations){
        for(const auto &vehicle : station->getVehicles()){ //look for vehicle at the stations:
            if(vehicle->getId() == id){
                found = true;
                printVehicleInfo(vehicle);
                std::cout << "Vehicle is currently at station: " << station->getName() << " (possibly attached to a train, see vehicle sim history)";
            }

        };
    }
    if(!found){
        for(const auto &train : trains){
                for(const auto &v : train->getVehicles()){
                    if(v->getId() == id && train->getState()!=FINISHED){
                        printVehicleInfo(v);
                        std::cout << "Vehicle is attached to train nr: " << train->getId() << " with state " << train->outputStateString(train->getState()) << std::endl;


                    }
                }
            }
        }
    returnToMenu();
    }


void TrainAndBrain::showAllVehicles() {
    for(const auto &station : stationList->stations){
        for(const auto &vehicle : station->getVehicles()){ //print all vehicles at each station
            printVehicleInfo(vehicle);
        };
    }
}


void TrainAndBrain::changeLogLevel() {


    std::cout << "Choose log level" << std::endl;
    std::cout << "1. LOW" <<std::endl;
    std::cout << "2. HIGH" <<std::endl;
    int answer;
    inputAndCheckIfInt(answer);
    //if user writes 1, set currentLogLevel to low, otherwise set it to high.
    answer == 1 ? currentLogLevel=LOW : currentLogLevel=HIGH;

    std::cout << "Log level changed to: " << logLevelNames[currentLogLevel] <<std::endl;


}

void TrainAndBrain::changeIntervalLength(){
    std::cout << "Enter interval length: " << std::endl;
    int interval;
    inputAndCheckIfInt(interval);
    theSim->setIntervalLength(interval);
}


void TrainAndBrain::scheduleTrains(){
    for(auto &train : trains){
        //set initial state for a new train
        train->setCurrentState(NOT_ASSEMBLED);
        //create the first event (assemble event)
        auto newAssembleEvent = std::make_shared<AssembleEvent>(theSim, train, stationHandler,
                                                                train->getFirstScheduledEventTime());
        theSim->scheduleEvent(newAssembleEvent);
    }
}

void TrainAndBrain::printTrainLogForTrain(int id){
    //print message history for train with the specified id
    for(const auto &train : trains){
        if(train->getId() == id){
            for(const auto &message : train->getMessageHistory()){
                std::cout << message << std::endl;
            }
        }
    }
}

void TrainAndBrain::delayedTrains(){
    std::cout << "Delayed trains " << std::endl;
    for(const auto &train : trains){
        //if we find a delayed train that has left the station:
        if(train->isDelayed() && train->hasLeftStation()){
            printTrainInfo(train);

        }
    }
}

void TrainAndBrain::printTrainInfo(const std::shared_ptr<Train> &train){
    printStarLine();
    std::cout << "**** INFORMATION FOR TRAIN NR " << train->getId() << " *****" << std::endl;
    printStarLine();
    std::cout << train->getData() << std::endl;
    std::cout << "Average speed for todays journey: " << train->getCurrentSpeed() << " (train max speed: " << train->getMaxSpeed() << ")" << std::endl;
    printLine();
    std::cout << "Vehicles that were/are attached to this train: " << std::endl;
    printLine();
    //print none if the train hasn't yet attached any vehicles
    if(train->getVehicles().empty()){
        std::cout << " none " << std::endl;
    } else {
        for(const auto &v : train->getVehicles()){
            //otherwise print info for the vehicles
            printVehicleInfo(v);
        }
    }
    printLine();
    std::cout << "Vehicle types not yet attached to train: ";
    if(train->getVehicleList().empty()){
        std::cout << " none. Train found all its vehicles." << std::endl;
    } else { //print vehicles types the train needs (and still hasn't found)
        for(auto type : train->getVehicleList()){
            switch(type) {
                case 0: std::cout << "Day Carriage | ";
                    break;
                case 1: std::cout << "Sleeper | ";
                    break;
                case 2: std::cout << "Open freight | ";
                    break;
                case 3: std::cout << "Covered freight | ";
                    break;
                case 4: std::cout << "Electric Engine | ";
                    break;
                case 5: std::cout << "Diesel Engine | ";
                    break;
                default:
                    break;
            }
        }
    }
    std::cout << std::endl;
    printLine();
    std::cout << "Carriage order for the complete train: " << std::endl;
    for(auto type : train->originalCarriageOrder){
        switch(type) {
            case 0: std::cout << "Day Carriage | ";
                break;
            case 1: std::cout << "Sleeper | ";
                break;
            case 2: std::cout << "Open freight Carriage | ";
                break;
            case 3: std::cout << "Closed freight Carriage | ";
                break;
            case 4: std::cout << "Electric Engine | ";
                break;
            case 5: std::cout << "Diesel Engine | ";
                break;
            default:
                break;
        }
    }
    std::cout << std::endl;
    printLine();
}

void TrainAndBrain::printFullInfoForTrain(int id){

    auto printTrainInfoLambda = [this, &id](std::shared_ptr<Train> &train){
        if(train->getId() == id){
            printTrainInfo(train);
        }
    };

    //print info for each train.
    std::for_each(trains.begin(), trains.end(), printTrainInfoLambda);
}

void TrainAndBrain::printTrainsThatWereOnTime(){
    std::cout << "Trains that were on time " << std::endl;
    for(const auto &train : trains){
        //for not-delayed trains that actually left the station:
        if(train->hasLeftStation() && !train->isDelayed() ){
            printTrainInfo(train);
        }


    }
}

void TrainAndBrain::printTrainsThatNeverLeftTheStation(){
    std::cout << "Trains that never left the station " << std::endl;
    for(const auto &train : trains){
        if(!train->hasLeftStation() ){
            printTrainInfo(train);
        }


    }
}

void TrainAndBrain::runFinishedMenu(bool &again){


    do {
        //update title time and log level display
        finishedMenu.setTitle("SIMULATION MENU [SIM FINISHED] " + theSim->getLastTimeHandled().getTimeString());
        finishedMenu.setItem(0 , "Change log level [" + std::string(logLevelNames[currentLogLevel]) + "]");

        finishedMenu.printMenuItems();

        switch (finishedMenu.getMenuChoice()) {
            case 1: changeLogLevel();
                break;
            case 2: printStatistics();
                break;
            case 3: runTrainMenu();
                break;
            case 4: runStationMenu();
                break;
            case 5: runVehicleMenu();
                break;
            case 6: again = false;
                break;
            default:
                break;
        }
    } while (again);
}

void TrainAndBrain::runSimulationMenu(){
    bool secondAgain = true;

    do {
        mainMenu.setTitle("SIMULATION MENU " + theSim->getLastTimeHandled().getTimeString());
        //update interval display
        mainMenu.setItem(0 , "Change interval length [" + std::to_string(theSim->getInterval()) + "]");
        mainMenu.setItem(4 , "Change log level [" + std::string(std::string(logLevelNames[currentLogLevel])) + "]");



        if(!theSim->simFinished()) {
            mainMenu.printMenuItems();
            switch (mainMenu.getMenuChoice()) {
                case 1:
                    changeIntervalLength();
                    break;
                case 2:
                    theSim->runNextInterval();
                    break;
                case 3:
                    theSim->nextEvent();
                    break;
                case 4:
                    theSim->run();
                    break;
                case 5:
                    changeLogLevel();
                    break;
                case 6:
                    runTrainMenu();
                    break;
                case 7:
                    runStationMenu();
                    break;
                case 8:
                    runVehicleMenu();
                    break;
                case 9:
                    showTimetable();
                    break;
                case 10:
                    secondAgain = false;
                    break;
                default:
                    break;
            }
        } else {
            runFinishedMenu(secondAgain);
        }
    } while (secondAgain);
}

void TrainAndBrain::showSimHistoryForTrain(){
    std::cout << "Enter train id: " << std::endl;
    int id;
    inputAndCheckIfInt(id);
    printTrainLogForTrain(id);
    returnToMenu();
}

void TrainAndBrain::runTrainMenu(){
    bool secondAgain = true;

    do {
        trainMenu.setTitle("TRAIN MENU");
        //update log level display
        trainMenu.setItem(4 , "Change log level [" + std::string(logLevelNames[currentLogLevel]) + "]");

        trainMenu.printMenuItems();

        switch (trainMenu.getMenuChoice()) {
            case 1: searchTrainByNumber();
                break;
            case 2: searchTrainByVehicleId();
                break;
            case 3: showAllTrains();
                break;
            case 4: showSimHistoryForTrain();
                break;
            case 5: changeLogLevel();
                break;
            case 6: secondAgain = false;
                break;
            default:
                break;
        }
    } while (secondAgain);
}

void TrainAndBrain::printTotalDelays(){
    std::cout << "Arrival delay sum in minutes (only trains that left station)" << std::endl;
    std::cout << getArrDelay().getTimeString() << std::endl;

    std::cout << "-----------------------" << std::endl;
    std::cout << "Departure delay sum in minutes (only trains that left station) " << std::endl;
    std::cout << getDepDelay().getTimeString() << std::endl;
    std::cout << "-----------------------" << std::endl;
    returnToMenu();
}

//statistics menu
void TrainAndBrain::printStatistics(){

    bool again = true;

    do {
        statsMenu.printMenuItems();

        switch (statsMenu.getMenuChoice()) {
            case 1:
                printTotalDelays();
                break;
            case 2:
                printTrainsThatNeverLeftTheStation();
                break;
            case 3:
                printTrainsThatWereOnTime();
                break;
            case 4: delayedTrains();
                break;
            case 5:
                again = false;
                break;
            default:
                break;
        }
    } while (again);

}

void TrainAndBrain::run() {
    try {

        //first read in necessary data from file
        readStationsFromFile();
        readTrainsFromFile();
        readTrainMapFromFile();
        scheduleTrains();

        bool secondAgain = true;

        do {
            //update menu items with variable display
            firstMenu.setItem(0, "Change start time [" + theSim->getChosenStart() + "]");
            firstMenu.setItem(1, "Change end time [" + theSim->getChosenEnd() + "]");
            firstMenu.printMenuItems();

            switch (firstMenu.getMenuChoice()) {
                case 1:
                    changeStartTime();
                    break;
                case 2:
                    changeEndTime();
                    break;
                case 3:
                    runSimulationMenu();
                    break;
                case 4:
                    secondAgain = false;
                    break;
                default:
                    break;
            }
        } while (secondAgain);


    } catch(std::runtime_error &re){
        //output the corresponding runtime_error if reading a file throws an exception
        std::cout << re.what() << " Check formatting of file" << std::endl;
    }
}

Time TrainAndBrain::getArrDelay(){
    Time sumTime;
    for(const auto &train : trains){
        if(train->isDelayed() && train->hasLeftStation()) {
            //calculate arrival delay for all trains
            sumTime += train->getDelay();
        }
    }
    return sumTime;
}

Time TrainAndBrain::getDepDelay(){
    Time sumTime;
    for(const auto &train : trains){
        if(train->trainWasDelayed() || (train->isDelayed() && train->hasLeftStation())) {
            //calculate departure deplay (called the "originalDelay")
            sumTime += train->getOriginalDelay();
        }
    }
    return sumTime;
}

void TrainAndBrain::showTimetable() {

    printStarLine();
    std::cout << "**** TIMETABLE ****" << std::endl;
    printStarLine();

    //create a new priority queue
    std::priority_queue<std::shared_ptr<Train>, std::vector<std::shared_ptr<Train>>, TimetableCompare> timetable;

    //function that moves a train to the timetable priority queue
    auto moveToPQ = [&timetable](std::shared_ptr<Train> &aTrain){
        timetable.emplace(aTrain);
    };

    //move each train to the PQ.
    std::for_each(trains.begin(), trains.end(), moveToPQ);

    //format output and output column titles
    std::cout << std::left;
    std::cout << std::setw(10) << "TRAIN NR" << std::setw(30) << "FROM" << std::setw(30) << "TO" << std::setw(10) << "DEPARTURE" << std::setw(10) << "ARRIVAL" << std::endl;
    printLine();
    //output times for each train until the timetable queue is empty
    while(!timetable.empty()){
        std::shared_ptr<Train> aTrain = timetable.top();
        timetable.pop();
        std::cout << std::setw(10) << aTrain->getId() << std::setw(30) << aTrain->getDepStation()->getName() << std::setw(30) << aTrain->getArrStation()->getName() << std::setw(10) << aTrain->getOriginalDepTime().getTimeString() << std::setw(10) << aTrain->getOriginalArrTime().getTimeString() << std::endl;

    };
}

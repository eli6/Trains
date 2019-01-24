//
// Created by Elin Fritiofsson on 2018-01-30.
// functionLib.cpp
//

#include "functionLib.h"

using namespace std;


void printStarLine(){
    cout << "*********************************************************************************************************************************" << endl;
}

void printLine(){
    cout << "---------------------------------------------------------------------------------------------------------------------------------" << endl;
}

void inputAndCheckIfInt(int &variable){
    bool correctInput = false;
    do {
        cin >> variable;
        cin.get();

        //Kontrollera om inmatningen lyckas
        if(!cin) {
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            cout << "Enter a number" << endl;
            correctInput = false;
        } else {
            correctInput = true;
        }
    } while(!correctInput);
}



void returnToMenu(){
    cout << "Press enter to return to the previous menu." << endl;
    cin.get();
}

void inputTime(Time &newTime) {
    int hour = 0;
    int minute = 0;
    std::string input;
    bool timeOK;

    do{
        timeOK = true;

        //holds number of delimiters (:) in input of time (2) xx:xx:xx or (1) xx:xx
        long delimiterCount;
        char TIME_DELIM = ':';
        do {
            cout << "Enter new time (m)m:ss" << endl;
            getline(std::cin, input);
            delimiterCount = std::count(input.begin(), input.end(), TIME_DELIM);
        } while (delimiterCount != 1);

        std::string hourString = "0";
        std::string minuteString = "0";

        //same with minutes (we start here if input is xx:xx)
        hourString = input.substr(0, input.find(TIME_DELIM));
        input.erase(0, input.find(TIME_DELIM) + 1);
        removeFirstZero(hourString);

        //use last part of input as value for seconds
        minuteString = input;
        removeFirstZero(minuteString);


        try {
            //try to convert the strings to int
            hour = stoi(hourString);
            minute = stoi(minuteString);
        } catch (std::exception &e) {
            cout << "Wrong time format. You will have to enter the time again" << endl;
            timeOK = false;
        }

        //check that the time units have reasonable value
        if((hour < 0 || hour >= 24) || (minute > 60 || minute < 0)){
            cout << "Time must be a positive number, not after 23:59, and minutes below 60!" << endl;
            timeOK = false;
        }

    } while (!timeOK);

    int totalTime = (hour*60)+minute;

    newTime.setTime(totalTime);

}


void removeFirstZero(std::string &timeUnit) {
    if (timeUnit[0] == '0' && timeUnit.length() > 1) {
        timeUnit = timeUnit[1];
    }
}
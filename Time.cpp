//
// Created by Elin Fritiofsson on 2018-05-24.
//


#include "Time.h"

int Time::convertToMinutes(const int timmar, const int minuter) {
        int totalMinutes = (timmar*60)+minuter;
        return totalMinutes;
}

void Time::setTime( int newTotalTime ) {
        totalMinutes = newTotalTime;
        hours = totalMinutes/60;
        minutes = totalMinutes%60;
}

void Time::addTime( int newTotalTime){
        totalMinutes += newTotalTime;
        hours = totalMinutes/60;
        minutes = totalMinutes%60;

}

void Time::resetToZero(){
        totalMinutes = 0;
        hours = 0;
        minutes = 0;
}




std::string Time::getTimeString() {
        std::string hourString = std::to_string(hours);
        std::string minuteString = std::to_string(minutes);

        if(hours < 10){
                hourString = "0" + std::to_string(hours);
        }
        if(minutes < 10){
                minuteString = "0" + std::to_string(minutes);
        }
        if(hours == 24){
                std::string days = std::to_string(hours/24);
                hourString = std::to_string(hours%24);
                if(hours%24 < 10){
                        hourString = "0" + hourString;
                }
                std::string returnString  = days + ":" + hourString + ":" + minuteString;
                return returnString;
        }


        return (hourString + ":" + minuteString);
}
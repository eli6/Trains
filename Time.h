//
// Created by Elin Fritiofsson on 2018-05-24.
//

#ifndef UNTITLED1_TIME_H
#define UNTITLED1_TIME_H

#include <string>
#include <iostream>

class Time {
private:
    int totalMinutes; /**< time counted in minutes from midnight */
    int hours;
    int minutes;
    int convertToMinutes(const int timmar, const int minuter);
public:
    Time(){
        totalMinutes = 0;
        hours = 0;
        minutes = 0;
    }
    Time(int newHour, int newMinute) : hours(newHour), minutes(newMinute) {
        totalMinutes = convertToMinutes(newHour, newMinute);
    }
    Time(int newTotalMinutes){ /**< time constructor that only takes a totalminutes value*/
        totalMinutes = newTotalMinutes;
        hours = totalMinutes/60;
        minutes = totalMinutes%60;
    }

    //----------------------
    //BASIC SETTERS/GETTERS:
    //-----------------------
    void setTime( int newTotalTime );
    void addTime( int newTotalTime);

    int getTotalMinutes() const { return totalMinutes; };
    std::string getTimeString();

    /**
     * resets time to zero
     */
    void resetToZero();

    //----------------------
    //OVERRIDES:
    //----------------------
    bool operator>(const Time &t2){
        return getTotalMinutes() > t2.getTotalMinutes();
    }

    double operator-(const double &t2){
        return getTotalMinutes()-t2;
    }

    double operator+(const double &t2){
        return getTotalMinutes()+t2;
    }

    Time operator+(const Time &t2){
        totalMinutes += convertToMinutes(t2.hours, t2.minutes);
        hours = totalMinutes/60;
        minutes = totalMinutes%60;
        return *this;
    }

    Time& operator +=(const Time &t2){
        totalMinutes += t2.getTotalMinutes();
        hours = totalMinutes/60;
        minutes = totalMinutes%60;
        return *this;
    }

    double operator-(const Time &t2){
        return getTotalMinutes()-t2.getTotalMinutes();
    }
};



#endif //UNTITLED1_TIME_H

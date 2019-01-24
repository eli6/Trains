//
// Created by Elin Fritiofsson on 2018-01-30.
// functionLib.cpp | Contains helper functions used throughout the program
//

#ifndef LABBEN_FUNCTIONLIB_H
#define LABBEN_FUNCTIONLIB_H

#include<iostream>
#include<string>
#include<vector>
#include<limits>
#include "../Time.h"


/**
Layoutfunktion. Skriver ut en rad med stjärnor (***...)
*/
    void printStarLine();

    /**
    Layoutfunktion. Skriver ut en streckad rad (---...)
    */
    void printLine();

    /**
    Tilldelar ett värde till en int-variabel och kontrollerar att inmatningen lyckas.

     @param variable : variabeln som ska få ett värde
    */
    void inputAndCheckIfInt(int &variable);

    /**
    Ber användaren trycka på enter för att återgå till menyn
    */
    void returnToMenu();

    void inputTime(Time &newTime);

void removeFirstZero(std::string &timeUnit);
#endif //LABBEN_FUNCTIONLIB_H

//
// Created by Elin Fritiofsson on 2018-03-02.
//  Menu.cpp | Definition of Menu
//

#include "Menu.h"
using std::cout;
using std::endl;

void Menu::setItem(int index, const std::string &newString) {
    menuItems[index] = newString;

}

void Menu::setItems(std::vector<std::string> newMenuItems) {
    menuItems = newMenuItems;
}

void Menu::setTitle(std::string newTitle) {
    menuTitle = newTitle;
}

void Menu::printMenuItems() {
    cout << endl;
    printStarLine();
    cout << menuTitle << endl;
    printStarLine();
    cout << endl;

    //Print menu choices on screen.
    int counter = 0;
    for (auto &item: menuItems) {
            cout << ++counter << ". " << item << endl;

    }

    printLine();
}


int Menu::getMenuChoice() {

    int choice;
    cout << endl;
    cout << "Your choice: " << endl;
    do {
        inputAndCheckIfInt(choice);
        //check that user doesn't enter too high a number.
        if(choice > menuItems.size()){
            cout << "Choose one of the available menu items... " << endl;
        }
    } while(choice > menuItems.size());

    return choice;
}
//
// Created by Elin Fritiofsson on 2018-03-02.
// Menu.h | Interface for Menu
//

#ifndef KOD_MENU_H
#define KOD_MENU_H
#include <string>
#include <vector>
#include <iostream>
#include "FunctionLibrary/functionLib.h"

class Menu {
private:
    std::vector<std::string> menuItems; /**< the menu choices displayed on screen*/
    std::string menuTitle; /**< title for the menu */
public:
    Menu() = default;
    Menu(std::string &newMenuTitle, std::vector<std::string> &newItemsVector):
            menuTitle(newMenuTitle), menuItems(newItemsVector) {};
    void setTitle(std::string newTitle);
    void setItems(std::vector<std::string> newMenuItems);
    void setItem(int index, const std::string &newString);

    void printMenuItems(); /**< prints the menu choices on screen*/
    int getMenuChoice(); /**< reads the users choice, validates it, and returns its number value */
};


#endif //KOD_MENU_H

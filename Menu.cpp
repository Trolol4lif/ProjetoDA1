#include <iostream>
#include "Menu.h"

Menu::Menu() {
}

int Menu::run(){
    int option = menuOptions();
    while(option != 0){
        switch(option){
            case 1:
                break;
            case 2:
                break;
            default:
                std::cout << "Invalid option try again" << std::endl;
        }
        option = menuOptions();
    }
    exit();
    return 0;
}
/**
 * @brief Exits the menu and displays an exit message.
 */
void Menu::exit(){
    std::cout << ">> Exiting the menu." << std::endl;
}

/**
 * @brief Displays the main menu options.
 * @return The selected menu option.
 */

int Menu::menuOptions() const {
    int option;
    std::cout << "1. List Basic Service Metrics" << std::endl;
    std::cout << "2. List Basic Service Metrics" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your option:";
    std::cin >> option;
    return option;
}

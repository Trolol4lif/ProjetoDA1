#include <iostream>
#include "Menu.h"

Menu::Menu() {
}

int Menu::run(){
    int option = menuOptions();
    while(option != 0){
        switch(option){
            case 1:
                listBasicServiceMetrics();
                break;
            case 2:
                listReliabilityandSensitivitytoFailures();
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
    std::cout << "2. List Reliability and Sensitivity to Failures" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "Enter your option:";
    std::cin >> option;
    return option;
}


void Menu::listBasicServiceMetrics(){
    int option = basicServiceMetricsOptions();
    int checkMaxWaterCityOption;
    while (option){
        switch (option) {
            case 1:
                checkMaxWaterCityOption = checkMaxWaterCityOptions();
                while (checkMaxWaterCityOption){
                    switch (checkMaxWaterCityOption) {
                        case 1:
                            data.checkMaxWaterWholeNetwork();
                            break;
                        case 2:
                            data.checkMaxWaterCity();
                            break;
                        default:
                            std::cout << "Invalid option try again" << std::endl;
                    }
                    checkMaxWaterCityOption = checkMaxWaterCityOptions();
                }
                break;
            case 2:
                data.checkCitiesWaterDeficit();
                break;
            case 3:
                //TODO
                data.checkBefore_AfterBalancing();
                break;
            default:
                std::cout << "Invalid option try again" << std::endl;
        }
        option = basicServiceMetricsOptions();
    }
}

void Menu::listReliabilityandSensitivitytoFailures(){
    int option = reliabilityandSensitivitytoFailuresOptions();
    while (option){
        switch (option) {
            case 1:
                //TODO
                break;
            case 2:
                data.citiesAffectedPumpingStationRemoval();
                break;
            case 3:
                //TODO
                break;
            case 4:
                data.citiesAffectedPipelineRupture();
                break;
            default:
                std::cout << "Invalid option try again" << std::endl;
        }
        option = reliabilityandSensitivitytoFailuresOptions();
    }
}


int Menu::reliabilityandSensitivitytoFailuresOptions() const{
    int option;
    std::cout << "1. Check the affected cities whose water supply does not meet its demand when a reservoir is removed" << std::endl;
    std::cout << "2. Check which cities are affected when a pumping station is removed" << std::endl;
    cout << "3. Check for each City which pipeline's rupture will incapacitate the desired amount of water to be delivered" << endl;
    cout << "4. Check for each pipeline rupture which cities will be affected" << endl;
    cout<< "0. Back to main menu" << endl;
    cout << "Enter your option:";
    cin >> option;
    return option;
}

int Menu::basicServiceMetricsOptions() const{
    int option;
    std::cout << "1. Check the maximum amount of water that can reach each or a specific city" << std::endl;
    std::cout << "2. Check the Cities that don't meet the desired water rate level requirements" << std::endl;
    cout << "3. Check the average and variance of the differences between capacity and flow for each pipe, as well as,\n the maximum difference, before and after the balancing" << endl;
    cout<< "0. Back to main menu"<<endl;
    cout << "Enter your option:";
    cin >> option;
    return option;
}

int Menu::checkMaxWaterCityOptions() const{
    int option;
    std::cout << "1. Check for all cities" << std::endl;
    std::cout << "2. Check for a specific city" << std::endl;
    cout << "0. Go back" << endl;
    cout << "Enter your option:";
    cin >> option;
    return option;
}

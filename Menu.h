#ifndef PROJETODA1_MENU_H
#define PROJETODA1_MENU_H

#include "Data.h"

/**
 * @brief Class that serves as the interactive menu for the user
 */
class Menu {
private:
    /** @brief Attribute that can access all data processed */
    Data data;
public:
    Menu();
    int run();
private:
    /** Menu options */
    void exit();
    int menuOptions() const;
    int basicServiceMetricsOptions() const;
    int reliabilityandSensitivitytoFailuresOptions() const;
    void listReliabilityandSensitivitytoFailures();
    void listBasicServiceMetrics();
};
#endif //PROJETODA1_MENU_H

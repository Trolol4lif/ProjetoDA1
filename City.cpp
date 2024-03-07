//
// Created by pmigu on 07/03/2024.
//

#include "City.h"

const string &City::getName() const {
    return name;
}

void City::setName(const string &name) {
    City::name = name;
}

int City::getId() const {
    return id;
}

void City::setId(int id) {
    City::id = id;
}

const string &City::getDeliveryCode() const {
    return delivery_code;
}

void City::setDeliveryCode(const string &deliveryCode) {
    delivery_code = deliveryCode;
}

int City::getDemand() const {
    return demand;
}

void City::setDemand(int demand) {
    City::demand = demand;
}

int City::getPopulation() const {
    return population;
}

void City::setPopulation(int population) {
    City::population = population;
}

City::City(const std::string &name, const std::string &delivery_code, const int &demand, const int &population) {
    this->name = name;
    this->delivery_code = delivery_code;
    this->demand = demand;
    this->population = population;
}
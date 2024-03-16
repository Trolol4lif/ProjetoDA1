#include "City.h"

const string &City::getName() const {
    return name;
}

void City::setName(const string &name) {
    City::name = name;
}

double City::getDemand() const {
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

City::City(const string& name,const int& id,const string& delivery_code,const double& demand,const int& population): NodeData(id,delivery_code,nodeType::city){
    this->name = name;
    this->demand = demand;
    this->population = population;
}
// Change to code maybe???
bool City::operator==(const City& other) const{
    return id == other.id;
}
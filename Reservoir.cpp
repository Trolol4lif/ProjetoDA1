#include "Reservoir.h"


Reservoir::Reservoir(const std::string &name, const std::string &municipality, const int &id, const std::string &code,const int &maxDelivery): NodeData(id,code,reservoir) {
    this->name = name;
    this->municipality = municipality;
    this->maxDelivery = maxDelivery;
}

const string &Reservoir::getName() const {
    return name;
}

void Reservoir::setName(const string &name) {
    Reservoir::name = name;
}

const string &Reservoir::getMunicipality() const {
    return municipality;
}

void Reservoir::setMunicipality(const string &municipality) {
    Reservoir::municipality = municipality;
}

int Reservoir::getMaxDelivery() const {
    return maxDelivery;
}

void Reservoir::setMaxDelivery(int maxDelivery) {
    Reservoir::maxDelivery = maxDelivery;
}
// Change to code maybe???
bool Reservoir::operator==(const Reservoir& other) const{
    return other.id == id;
}

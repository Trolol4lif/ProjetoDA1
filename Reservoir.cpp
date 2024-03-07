#include "Reservoir.h"


Reservoir::Reservoir(const string &name, const string &municipality, const string &code, int maxDelivery) : name(name),municipality(municipality),code(code),maxDelivery(maxDelivery) {}

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

int Reservoir::getId() const {
    return id;
}

void Reservoir::setId(int id) {
    Reservoir::id = id;
}

const string &Reservoir::getCode() const {
    return code;
}

void Reservoir::setCode(const string &code) {
    Reservoir::code = code;
}

int Reservoir::getMaxDelivery() const {
    return maxDelivery;
}

void Reservoir::setMaxDelivery(int maxDelivery) {
    Reservoir::maxDelivery = maxDelivery;
}



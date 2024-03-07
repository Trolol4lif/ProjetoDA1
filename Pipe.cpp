#include "Pipe.h"


Pipe::Pipe(const string &servicePointA, const string &servicePointB, int capacity, bool direction) : servicePointA(
        servicePointA), servicePointB(servicePointB), capacity(capacity), direction(direction) {}

const string &Pipe::getServicePointA() const {
    return servicePointA;
}

void Pipe::setServicePointA(const string &servicePointA) {
    Pipe::servicePointA = servicePointA;
}

const string &Pipe::getServicePointB() const {
    return servicePointB;
}

void Pipe::setServicePointB(const string &servicePointB) {
    Pipe::servicePointB = servicePointB;
}

int Pipe::getCapacity() const {
    return capacity;
}

void Pipe::setCapacity(int capacity) {
    Pipe::capacity = capacity;
}

bool Pipe::isDirection() const {
    return direction;
}

void Pipe::setDirection(bool direction) {
    Pipe::direction = direction;
}


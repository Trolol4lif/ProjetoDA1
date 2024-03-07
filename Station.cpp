#include "Station.h"

Station::Station(){}

Station::Station(int id, string code):
        id(id), code(code){}


string Station::getCode() const{
    return code;
}

int Station::getId() const{
    return id;
}

void Station::setId(int id) {
    Station::id = id;
}

void Station::setCode(const string &code) {
    Station::code = code;
}

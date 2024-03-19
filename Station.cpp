#include "Station.h"

Station::Station(int id, string code):NodeData(id, code, nodeType::STATION){}

// Change to code maybe???

bool Station::operator==(const Station& other) const{
    return other.id == id;
}

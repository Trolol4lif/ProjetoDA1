#include "Station.h"

Station::Station(int id, string code):NodeData(id, code, nodeType::station){}

// Change to code maybe???

bool Station::operator==(const Station& other) const{
    return other.id == id;
}

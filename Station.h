#ifndef STATION_H
#define STATION_H

#include <iostream>
#include <string>
#include "NodeData.h"

using namespace std;

class Station: public NodeData{
    public:
        Station(int id, string code);
        bool operator==(const Station& other) const;
};


#endif

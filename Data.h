#ifndef PROJETODA1_DATA_H
#define PROJETODA1_DATA_H

#define CITIES "../dataset/Cities.csv"
#define PIPES "../dataset/Pipes.csv"
#define RESERVOIR "../dataset/Reservoir.csv"
#define STATIONS "../dataset/Stations.csv"

#include <unordered_map>
#include "City.h"
#include "Reservoir.h"
#include "Pipe.h"
#include "Station.h"
#include "data_structures/Graph.h"
#include "NodeData.h"

class Data {
    public:
        Data();
        void read_cities();
        void read_pipes();
        void read_reservoir();
        void read_stations();
    private:
        Graph<NodeData>* waterG;
        unordered_map<string,NodeData*> nodes;
        unordered_map<int,Reservoir*> reservoirs;
        unordered_map<int,City*> cities;
        unordered_map<int,Station*> stations;
};


#endif //PROJETODA1_DATA_H

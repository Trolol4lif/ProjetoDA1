#ifndef PROJETODA1_DATA_H
#define PROJETODA1_DATA_H

#define CITIES "../dataset/Cities.csv"
#define PIPES "../dataset/Pipes.csv"
#define RESERVOIRS "../dataset/Reservoir.csv"
#define STATIONS "../dataset/Stations.csv"

#include <unordered_map>
#include "NodeData.h"
#include "City.h"
#include "Reservoir.h"
#include "Pipe.h"
#include "Station.h"
#include "data_structures/Graph.h"


class Data {
    public:
        Data();
        void read_cities();
        void read_pipes();
        void read_reservoir();
        void read_stations();
        void checkMaxWaterCity();

    void checkCitiesWaterDeficit();

private:
        Graph<NodeData*>* waterG;
        unordered_map<string,Vertex<NodeData*>*> nodes;
        unordered_map<int,Vertex<NodeData*>*> reservoirs;
        unordered_map<int,Vertex<NodeData*>*> cities;
        unordered_map<int,Vertex<NodeData*>*> stations;
};


#endif //PROJETODA1_DATA_H

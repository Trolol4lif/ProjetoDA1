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
        void deepCopyGraph(Graph<NodeData *> *graph, unordered_map<string, Vertex<NodeData *> *> oldmap,unordered_map<string, Vertex<NodeData *> *> &newmap);
        void createEdmondskarpG();
        void addSuperSource_Sink(Graph<NodeData *> *graph, unordered_map<string, Vertex<NodeData *> *> &map);
        void checkMaxWaterWholeNetwork();
        void checkBefore_AfterBalancing();
        void print_calculateStatistics(Graph<NodeData *> *graph,double nPipes);
        Graph<NodeData *> * balancePipes(Graph<NodeData *> *pGraph);
        void citiesAffectedPipelineRupture();
        void citiesAffectedPumpingStationRemoval();
private:
        Graph<NodeData*>* waterG;
        Graph<NodeData*>* edmondskarpG;
        unordered_map<string,Vertex<NodeData*>*> nodesKarpG;
        unordered_map<string,Vertex<NodeData*>*> nodes;
        unordered_map<int,Vertex<NodeData*>*> reservoirs;
        unordered_map<int,Vertex<NodeData*>*> cities;
        unordered_map<int,Vertex<NodeData*>*> stations;
        unordered_map<int,Pipe*> pipes;

};


#endif //PROJETODA1_DATA_H

#include <fstream>
#include <sstream>
#include "Data.h"
#include "data_structures/Graph.h"
#include "NodeData.h"


Data::Data() {
    waterG = new Graph<NodeData>();
    read_cities();
    read_stations();
    read_reservoir();
    read_pipes();
}

void Data::read_cities() {
    string name,code;
    int population,id;
    double demand;
    std::ifstream file(CITIES);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            istringstream iss(line);
            getline(iss,name,',');
            string id_str,population_str,demand_str;
            getline(iss,id_str,',');
            getline(iss,code,',');
            getline(iss,demand_str,',');
            getline(iss,population_str,',');
            id = stoi(id_str);
            population = stoi(population_str);
            demand = stod(demand_str);
            NodeData* nodeData = new City(name,id,code,demand,population);
            waterG->addVertex(*nodeData);
            nodes[code] = nodeData;
        }
    }else {
        cout << "Could not open the file\n";
    }
}

void Data::read_pipes() {
    bool direction;
    string pointA,pointB;
    int capacity;
    std::ifstream file(PIPES);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string capacity_str,direction_str;
            istringstream iss(line);
            getline(iss,pointA,',');
            getline(iss,pointB,',');
            getline(iss,capacity_str,',');
            getline(iss,direction_str,',');
            direction = stoi(direction_str);
            capacity = stoi(capacity_str);
            if(direction){
                waterG->addEdge(*nodes[pointA],*nodes[pointB],capacity);
            }else{
                waterG->addBidirectionalEdge(*nodes[pointA],*nodes[pointB],capacity);
            }
        }
    }else {
        cout << "Could not open the file\n";
    }
}

void Data::read_reservoir() {
    string name,code,municipality;
    int id,maxDelivery;
    std::ifstream file(RESERVOIR);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string id_str,maxDelivery_str;
            istringstream iss(line);
            getline(iss,name,',');
            getline(iss,municipality,',');
            getline(iss,id_str,',');
            getline(iss,code,',');
            getline(iss,maxDelivery_str,',');
            id = stoi(id_str);
            maxDelivery = stoi(maxDelivery_str);
            NodeData* nodeData = new Reservoir(name,municipality,id,code,maxDelivery);
            waterG->addVertex(*nodeData);
            nodes[code] = nodeData;
        }
    }else {
        cout << "Could not open the file\n";
    }
}

void Data::read_stations() {
    string code;
    int id;
    std::ifstream file(STATIONS);
    if(file.is_open()){
        string line;
        getline(file,line); // Skip header line
        while (getline(file,line)){
            string id_str;
            istringstream iss(line);
            getline(iss,id_str,',');
            getline(iss,code,',');
            id = stoi(id_str);
            NodeData* nodeData = new Station(id,code);
            waterG->addVertex(*nodeData);
            nodes[code] = nodeData;
        }
    }else {
        cout << "Could not open the file\n";
    }
}
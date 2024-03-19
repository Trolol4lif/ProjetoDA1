#include <fstream>
#include <sstream>
#include "Data.h"
#include "data_structures/Graph.h"
#include "NodeData.h"


Data::Data() {
    waterG = new Graph<NodeData*>();
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
            City *city = new City(name,id,code,demand,population);
            Vertex<NodeData*>* l = waterG->addReturnVertex(city);
            cities[id] = l;
            nodes[code] = l;
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
                waterG->addEdge(nodes[pointA]->getInfo(),nodes[pointB]->getInfo(),capacity);
            }else{
                waterG->addBidirectionalEdge(nodes[pointA]->getInfo(),nodes[pointB]->getInfo(),capacity);
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
            Reservoir* reservoir = new Reservoir(name,municipality,id,code,maxDelivery);
            Vertex<NodeData*>* l = waterG->addReturnVertex(reservoir);
            reservoirs[id] = l;
            nodes[code] = l;
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
            Station* station = new Station(id,code);
            Vertex<NodeData*>* l = waterG->addReturnVertex(station);
            nodes[code] = l;
            stations[id] = l;
        }
    }else {
        cout << "Could not open the file\n";
    }
}

void Data::checkMaxWaterCity(){
}

void Data::checkCitiesWaterDeficit() {
    for(pair<int,Vertex<NodeData*>*> tuple: cities){
        Vertex<NodeData*>* city = tuple.second;
        int capacity = 0;
        NodeData* node = city->getInfo();
        for( Edge<NodeData*>* edge: city->getIncoming()){
            capacity += edge->getWeight();
        }
        City* pCity = (City*) node;
        if(capacity <  pCity->getDemand()){
            cout << "City code:" << pCity->getCode()  << " | Value:"  << pCity->getDemand() - capacity << endl;
        }
    }
}

template <class T>
double bfs(Graph<T>*g,int source,int target){
    for(Vertex<T>* v:g->getVertexSet()){
        v->setPath(nullptr);
    }
    std::queue<std::pair<Vertex<T>*,double>> queue;
    queue.push({g->findVertex(source),std::numeric_limits<double>::max()});
    while (!queue.empty()){
        Vertex<T>* v = queue.front().first;
        double flow = queue.front().second;
        queue.pop();

        for (Edge<T>* edge : v->getAdj()){
            Vertex<T>* dest = edge->getDest();
            if ((dest->getPath()== nullptr) && (edge->getWeight() > edge->getFlow())){
                dest->setPath(edge);
                double new_flow = std::min(flow, edge->getWeight());
                if (dest->getInfo() == target) return new_flow;
                queue.push({dest, new_flow});
            }
        }
        for (Edge<T>* edge : v->getIncoming()){
            Vertex<T>* orig = edge->getOrig();
            if ((orig->getPath()== nullptr) && (edge->getFlow() > 0)){
                orig->setPath(edge);
                double new_flow = std::min(flow, edge->getFlow());
                if (orig->getInfo() == target) return new_flow;
                queue.push({orig, new_flow});
            }
        }
    }
    return 0;
}
template <class T>
void edmondsKarp(Graph<T> *g, int source, int target) {
    // initialization
    for(Vertex<T>* v:g->getVertexSet()){
        v->setVisited(false);
        v->setProcesssing(false);
        for(Edge<T>* edge:v->getAdj()){
            edge->setFlow(0);
        }
    }
    double flow;
    // Search for augmenting paths
    while((flow = bfs(g,source,target))){
        // sum of residual water
        Vertex<T>* currentVertex = g->findVertex(target);
        while (currentVertex->getInfo() != source){
            Edge<T>* previous = currentVertex->getPath();
            if(previous->getDest() == currentVertex){
                previous->setFlow(previous->getFlow()+flow);
                currentVertex = previous->getOrig();
            } else{
                previous->setFlow(previous->getFlow()-flow);
                currentVertex = previous->getDest();
            }
        }
    }
}



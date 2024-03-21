#include <fstream>
#include <sstream>
#include <cmath>
#include <climits>
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
    std::ifstream file(RESERVOIRS);
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

std::pair<Edge<NodeData*>*,double> edBFS(NodeData* target,   Vertex<NodeData*> *source_v) {
    std::queue<std::pair<Vertex<NodeData*>*,double>> q;
    q.push({source_v,INT_MAX});
    source_v->setVisited(true);
    while(!q.empty()){
        auto v = q.front().first;
        double runningMin = q.front().second;
        q.pop();
        for(Edge<NodeData*>* edge: v->getAdj()){
            if(edge->getWeight() - edge->getFlow() > 0){
                if(edge->getDest()->getInfo() == target){
                    return {edge,std::min(edge->getWeight() - edge->getFlow(),runningMin)};
                }
                else if(!(edge->getDest()->isVisited())){
                    q.push({edge->getDest(),std::min(edge->getWeight() - edge->getFlow(),runningMin)});
                    edge->getDest()->setPath(edge);
                    edge->getDest()->setVisited(true);


                }
            }

        }
        for(Edge<NodeData*>* edge:v->getIncoming()){
            if(edge->getFlow() > 0){
                if(edge->getOrig()->getInfo() == target){
                    return {edge,std::min(edge->getFlow(),runningMin)};
                }
                else if(!(edge->getOrig()->isVisited())){
                    q.push({edge->getOrig(),std::min(edge->getFlow(),runningMin)});
                    edge->getOrig()->setPath(edge);
                    edge->getOrig()->setVisited(true);
                }
            }
        }
    }
    return {nullptr,0};
}


void edmondsKarpWalkback(std::pair<Edge<NodeData*>*,double> edge,NodeData* source,Vertex<NodeData*>* currentVertex){
    double minFlow = edge.second;
    Edge<NodeData*>* currentEdge = edge.first;


    while(currentEdge != nullptr){
        if(currentEdge->getDest() == currentVertex){ // normal way
            currentEdge->setFlow(currentEdge->getFlow()+minFlow);
            currentVertex = currentEdge->getOrig();
        }
        else{
            currentEdge->setFlow(currentEdge->getFlow()-minFlow);
            currentVertex = currentEdge->getDest();
        }
        currentEdge = currentVertex->getPath();

    }



}
template <class T>
void edmondsKarp(Graph<T> &g, NodeData* source, NodeData* target) {
    int max_flow = 0;
    auto source_v = g.findVertex(source);
    auto target_v = g.findVertex(target);
    for( Vertex<T>* v:g.getVertexSet()){

        for(Edge<T>* edge: v->getAdj()){
            edge->setFlow(0);
        }

    }
    while(true){
        for(Vertex<T>* v: g.getVertexSet()){
            v->setVisited(false);
            v->setPath(nullptr);
        }
        auto edge = edBFS(target,source_v);
        if( edge.first == nullptr){
            break;
        }
        edmondsKarpWalkback(edge,source,target_v);
    }
}

void Data::checkMaxWaterCity(){
    cout << "Type the id of the respective city:";
    int id;
    cin >> id;
    if(id > cities.size()){
        cout << "There is no city with such id" << endl;
        return;
    }
    Graph<NodeData*> graph = *waterG;
    //Add a superSource to the graph
    NodeData* source = new NodeData(0, "superSource", RESERVOIR);
    Vertex<NodeData*>* vertex = graph.addReturnVertex(source);
    // set the capacity of the SuperSource->Reservoir INFINITY
    for(auto pair:reservoirs){
        auto w = pair.second;
        vertex->addEdge(graph.findVertex(reservoirs[w->getInfo()->getId()]->getInfo()),INFINITY);
    }
    NodeData* sink = new NodeData(0, "superSink", CITY);
    Vertex<NodeData*>* sinkVertex = graph.addReturnVertex(sink);

    for(auto pair:cities){
        auto w = pair.second;
        graph.findVertex(cities[w->getInfo()->getId()]->getInfo())->addEdge(sinkVertex,INFINITY);
    }

    edmondsKarp(graph, source, sink);
    Vertex<NodeData*>* node = graph.findVertex(sink);
    int maxFlow = 0;
    for(auto edge:node->getIncoming()){
        maxFlow+=edge->getFlow();
    }

    cout << "Max water to arrive from city, " << ", is " << maxFlow << endl;
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




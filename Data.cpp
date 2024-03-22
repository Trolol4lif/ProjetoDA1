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
    createEdmondskarpG();
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
    int id = 1;
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
            pipes[id] = new Pipe(pointA,pointB,capacity,direction);
            if(direction){
                waterG->addEdge(nodes[pointA]->getInfo(),nodes[pointB]->getInfo(),capacity);
            }else{
                waterG->addBidirectionalEdge(nodes[pointA]->getInfo(),nodes[pointB]->getInfo(),capacity);
            }
            id++;
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


void edmondsKarpWalkback(std::pair<Edge<NodeData*>*,double> edge,Vertex<NodeData*>* currentVertex){
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
        edmondsKarpWalkback(edge,target_v);
    }
}

void Data::deepCopyGraph(Graph<NodeData*>* graph,unordered_map<string,Vertex<NodeData*>*> &map){
    for(auto pair:nodes){
        auto v = pair.second;
        NodeData* nodeData;
        switch (v->getInfo()->getType()) {
            case CITY:
                nodeData = new City(*(City*)v->getInfo());
                break;
            case RESERVOIR:
                nodeData = new Reservoir(*(Reservoir*)v->getInfo());
                break;
            case STATION:
                nodeData = new Station(*(Station*)v->getInfo());
                break;
            default:
                cout << "Invalid type" << endl;
                break;
        }
        Vertex<NodeData*>* vertex = graph->addReturnVertex(nodeData);
        map[v->getInfo()->getCode()] =  vertex;
    }
    for(auto pair:pipes){
        auto p = pair.second;
        if(p->isDirection()){
            graph->addEdge(map[p->getServicePointA()]->getInfo(),map[p->getServicePointB()]->getInfo(),p->getCapacity());
        }else{
            graph->addBidirectionalEdge(map[p->getServicePointA()]->getInfo(),map[p->getServicePointB()]->getInfo(),p->getCapacity());
        }
    }

}

void Data::createEdmondskarpG(){
    edmondskarpG = new Graph<NodeData*>();
    deepCopyGraph(edmondskarpG,nodesKarpG);
    addSuperSource_Sink(edmondskarpG,nodesKarpG);
    edmondsKarp(*edmondskarpG, nodesKarpG["superSource"]->getInfo(),nodesKarpG["superSink"]->getInfo());
}

void Data::addSuperSource_Sink(Graph<NodeData*>* graph,unordered_map<string,Vertex<NodeData*>*> &map){
    NodeData* source = new NodeData(0, "superSource", RESERVOIR);
    Vertex<NodeData*>* vertex = graph->addReturnVertex(source);
    // Add new source to Map
    map[vertex->getInfo()->getCode()] = vertex;
    // set the capacity of the SuperSource->Reservoir MaxDelivery of Reservoir
    for(auto pair:reservoirs){
        auto w = pair.second;
        Reservoir* reservoir = (Reservoir*) w->getInfo();
        vertex->addEdge(graph->findVertex(map[w->getInfo()->getCode()]->getInfo()),reservoir->getMaxDelivery());
    }
    // SuperSink
    NodeData* sink = new NodeData(0, "superSink", CITY);
    Vertex<NodeData*>* sinkVertex = graph->addReturnVertex(sink);
    map[sinkVertex->getInfo()->getCode()] = sinkVertex;
    for(auto pair:cities){
        auto w = pair.second;
        graph->findVertex(map[w->getInfo()->getCode()]->getInfo())->addEdge(sinkVertex,INFINITY);
    }
}

void Data::checkMaxWaterWholeNetwork() {
    Vertex<NodeData*>* node = edmondskarpG->findVertex(nodesKarpG["superSink"]->getInfo());
    int maxFlow = 0;
    for(auto edge:node->getIncoming()){
        maxFlow+=edge->getFlow();
    }
    cout << "The Max flow of water in the whole network is: " << maxFlow <<  endl;
}

void Data::checkMaxWaterCity(){
    cout << "Type the id of the respective city:";
    int id;
    cin >> id;
    if(id > cities.size()){
        cout << "There is no city with such id" << endl;
        return;
    }
    string code = "C_"+ to_string(id);
    Vertex<NodeData*>* node = edmondskarpG->findVertex(nodesKarpG[code]->getInfo());
    int maxFlow = 0;
    for(auto edge:node->getIncoming()){
        maxFlow+=edge->getFlow();
    }
    City* pCity = (City*) node->getInfo();
    cout << "Max water to arrive to city, " << pCity->getName() << " with code,"<< pCity->getCode() << ", is " << maxFlow <<  endl;
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




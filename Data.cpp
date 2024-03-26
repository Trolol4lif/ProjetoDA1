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

void resetFlow(const Graph<NodeData*>* g) {
    for( Vertex<NodeData*>* v:g->getVertexSet()){

        for(Edge<NodeData*>* edge: v->getAdj()){
            edge->setFlow(0);
        }

    }
}

void Data::deepCopyGraph(Graph<NodeData*>* graph,unordered_map<string,Vertex<NodeData*>*> oldmap,unordered_map<string,Vertex<NodeData*>*> &newmap){
    for(auto pair:oldmap){
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
        newmap[v->getInfo()->getCode()] =  vertex;

    }
    for(auto pair:oldmap){
        Vertex<NodeData*>* v = pair.second;
        for(auto edge: v->getAdj()){
            Edge<NodeData*>* newEdge = newmap[v->getInfo()->getCode()]->addEdge(newmap[edge->getDest()->getInfo()->getCode()],edge->getWeight());
            newEdge->setFlow(edge->getFlow());
            newEdge->setReverse(edge->getReverse());
        }
    }


}
// Function to test the given vertex 'w' and visit it if conditions are met
template <class T>
void testAndVisit(std::queue< Vertex<T>*> &q, Edge<T> *e, Vertex<T> *w, double residual) {
// Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
// Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}
// Function to find an augmenting path using Breadth-First Search
template <class T>
bool findAugmentingPath(Graph<T> *g, Vertex<T> *s, Vertex<T> *t) {
// Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v->setVisited(false);
    }
// Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex<T> *> q;
    q.push(s);
// BFS to find an augmenting path
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
// Process outgoing edges
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getWeight() - e->getFlow());
        }
// Process incoming edges
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
// Return true if a path to the target is found, false otherwise
    return t->isVisited();
}
// Function to find the minimum residual capacity along the augmenting path
template <class T>
double findMinResidualAlongPath(Vertex<T> *s, Vertex<T> *t) {
    double f = INFINITY;
// Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getWeight() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
// Return the minimum residual capacity
    return f;
}
// Function to augment flow along the augmenting path with the given flow value
template <class T>
void augmentFlowAlongPath(Vertex<T> *s, Vertex<T> *t, double f) {
// Traverse the augmenting path and update the flow values accordingly
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}
// Main function implementing the Edmonds-Karp algorithm
template <class T>
void edmondsKarp(Graph<T> *g, NodeData* source, NodeData* target) {
// Find source and target vertices in the graph
    Vertex<T>* s = g->findVertex(source);
    Vertex<T>* t = g->findVertex(target);
// Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");
// While there is an augmenting path, augment the flow along the path
    while( findAugmentingPath(g, s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }
}

void Data::createEdmondskarpG(){
    edmondskarpG = new Graph<NodeData*>();
    deepCopyGraph(edmondskarpG,nodes,nodesKarpG);
    addSuperSource_Sink(edmondskarpG,nodesKarpG);
    resetFlow(edmondskarpG);
    edmondsKarp(edmondskarpG,nodesKarpG["superSource"]->getInfo(),nodesKarpG["superSink"]->getInfo());
}

void Data::addSuperSource_Sink(Graph<NodeData*>* graph,unordered_map<string,Vertex<NodeData*>*> &map){
    Reservoir* source = new Reservoir("superSource","superSource",-1,"superSource",INFINITY);
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
    City* sink = new City("superSink",-1,"superSink",INFINITY,0);
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
    if(id > cities.size() || id < 1){
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

void Data::print_calculateStatistics(Graph<NodeData*>* graph,double nPipes){
    //TODO Refazer esta merda
    double average = 0;
    double maxdif = 0;
    double mindif = INFINITY;
    for(auto v:graph->getVertexSet()){
        if(v->getInfo()->getCode() == "superSource" || v->getInfo()->getCode() == "superSink"){
            continue;
        }
        for(auto e:v->getAdj()){
            if(e->getDest()->getInfo()->getCode() == "superSource" || e->getDest()->getInfo()->getCode() == "superSink" ){
                continue;
            }
            double difference = (e->getWeight() - e->getFlow())/e->getWeight();
            if(maxdif < difference){
                maxdif = difference;
            }
            if(mindif > difference){
                mindif = difference;
            }
            average+= difference;
        }
    }
    average/=nPipes*100;
    double variance = maxdif - mindif;
    cout << "The average difference of capacity-flow is " << average << ", having variance of " << variance << " and max difference " << maxdif << endl;
}

void Data::checkBefore_AfterBalancing(){
    cout << "---Before Balancing---" << endl;
    print_calculateStatistics(edmondskarpG,pipes.size());
    Graph<NodeData*>* balenced = balancePipes(edmondskarpG);

    cout << "---After using the Balancing algorithm---" << endl;
    print_calculateStatistics(balenced,pipes.size());
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

Graph<NodeData *> * Data::balancePipes(Graph<NodeData *> *pGraph) {
    Graph<NodeData*> *balanced = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(balanced,nodesKarpG,nodeMap);
    // remove original supersource and supersink
     balanced->removeVertex(nodeMap["superSource"]->getInfo()) ;
    balanced->removeVertex(nodeMap["superSink"]->getInfo());
    // add supersource
    Reservoir* source = new Reservoir("superSource","superSource",-1,"superSource",INFINITY);
    Vertex<NodeData*>* sourceVertex = balanced->addReturnVertex(source);
    nodeMap["superSource"] = sourceVertex;
    // add supersink
    City* sink = new City("superSink",-1,"superSink",INFINITY,0);
    Vertex<NodeData*>* sinkVertex = balanced->addReturnVertex(sink);
    nodeMap["superSink"] = sinkVertex;

    // connect supersink and supersource to graph
    for(auto pair: cities){
        auto city = nodeMap[pair.second->getInfo()->getCode()];
        double flow = 0;
        for( auto edge: city->getIncoming()){
            flow += edge->getFlow();
        }
        City* city1 = (City*) city->getInfo();
        if(city1->getDemand() < flow){
            auto added = sourceVertex->addEdge(nodeMap[city->getInfo()->getCode()],flow - city1->getDemand());
            added->setFlow(0);
        }
        if(city1->getDemand() > flow){
            auto added = nodeMap[city->getInfo()->getCode()]->addEdge(sinkVertex,INFINITY);
            added->setFlow(0);
        }
    }
    edmondsKarp(balanced,source,sink);



    return balanced;
}

void Data::citiesAffectedPipelineRupture(){
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    stringstream stream;
    int id = 1;
    for(auto pipe:pipes){
        if(pipe.second->isDirection()){
            graph->removeEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo());
        }else{
            graph->removeEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo());
            graph->removeEdge(nodeMap[pipe.second->getServicePointB()]->getInfo(),nodeMap[pipe.second->getServicePointA()]->getInfo());
        }
        resetFlow(graph);
        edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
        while (cities.size() != id){
            string code = "C_"+ to_string(id);
            City* pcity = (City*)nodeMap[code]->getInfo();
            int flow = 0;
            int oldFlow = 0;
            for(auto e:nodeMap[code]->getIncoming()){
                flow+=e->getFlow();
            }
            for(auto e:nodesKarpG[code]->getIncoming()){
                oldFlow+=e->getFlow();
            }
            if(oldFlow >= pcity->getDemand() && flow < pcity->getDemand()){
                stream << "/* Pipe rupture from " << pipe.second->getServicePointA() << "--" << pipe.second->getServicePointB() << " */" <<endl;
                stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << pcity->getDemand() - flow << endl;
            }
            id++;
        }
        if(pipe.second->isDirection()){
            graph->addEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo(),pipe.second->getCapacity());
        }
        else{
            graph->addBidirectionalEdge(nodeMap[pipe.second->getServicePointA()]->getInfo(),nodeMap[pipe.second->getServicePointB()]->getInfo(),pipe.second->getCapacity());
        }
        id = 1;
    }
    if(stream.str().empty()){
        cout << "There is no city affected by this pipe" << endl;
    }else{
        cout << stream.str();
    }
}

void Data::citiesAffectedWaterReservoirRemoval(){
    cout << "Type the id of the reservoir you want to remove:";
    int resevoirID;
    cin >> resevoirID;
    cout << endl;
    if(resevoirID > reservoirs.size() || resevoirID < 1){
        cout << "There is no reservoir with such id" << endl;
        return;
    }
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    int id = 1;
    stringstream stream;
    Vertex<NodeData*>* oldVertex = nodeMap["R_"+ to_string(resevoirID)];
    NodeData* newVertexInfo = new Reservoir(*(Reservoir*)oldVertex->getInfo());
    graph->removeVertex(oldVertex->getInfo());
    resetFlow(graph);
    edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
        while (cities.size() != id){
            string code = "C_"+ to_string(id);
            City* pcity = (City*)nodeMap[code]->getInfo();
            int flow = 0;
            int oldFlow = 0;
            for(auto e:nodeMap[code]->getIncoming()){
                flow+=e->getFlow();
            }
            for(auto e:nodesKarpG[code]->getIncoming()){
                oldFlow+=e->getFlow();
            }
            Reservoir* preservoir = (Reservoir*)newVertexInfo;
            if(oldFlow >= pcity->getDemand() && flow < pcity->getDemand()){
                stream << "/* Reservoir out of service " << preservoir->getName() << " with code " << preservoir->getCode() << " */"<< endl;
                stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << pcity->getDemand() - flow << endl;
            }
            id++;
        }
    if(stream.str().empty()){
        cout << "There is no city affected by the removal of any station" << endl;
    }else{
        cout << stream.str();
    }
}

void Data::citiesAffectedPumpingStationRemoval(){
    Graph<NodeData*> *graph = new Graph<NodeData*>();
    unordered_map<string,Vertex<NodeData*>*> nodeMap;
    deepCopyGraph(graph,nodes,nodeMap);
    addSuperSource_Sink(graph,nodeMap);
    int id = 1;
    stringstream stream;
    for(auto station:stations){
        Vertex<NodeData*>* oldVertex = nodeMap[station.second->getInfo()->getCode()];
        NodeData* newVertexInfo = new Station(*(Station*)oldVertex->getInfo());
        vector<pair<Vertex<NodeData*>*,double>> incoming;
        vector<pair<Vertex<NodeData*>*,double>> adj;
        for(auto e:oldVertex->getIncoming()){
            incoming.push_back({e->getOrig(),e->getWeight()});
        }
        for(auto e:oldVertex->getAdj()){
            adj.push_back({e->getDest(),e->getWeight()});
        }

        graph->removeVertex(oldVertex->getInfo());
        resetFlow(graph);
        edmondsKarp(graph,nodeMap["superSource"]->getInfo(),nodeMap["superSink"]->getInfo());
        while (cities.size() != id){
            string code = "C_"+ to_string(id);
            City* pcity = (City*)nodeMap[code]->getInfo();
            int flow = 0;
            int oldFlow = 0;
            for(auto e:nodeMap[code]->getIncoming()){
                flow+=e->getFlow();
            }
            for(auto e:nodesKarpG[code]->getIncoming()){
                oldFlow+=e->getFlow();
            }
            if(oldFlow >= pcity->getDemand() && flow < pcity->getDemand()){
                stream << "/* Station out of service " << station.second->getInfo()->getCode() << " */"<< endl;
                stream << "The city " << pcity->getName() << " with code " << pcity->getCode() << " is affected with a deficit of " << pcity->getDemand() - flow << endl;
            }
            id++;
        }
        // Reset Pumping Station
        Vertex<NodeData*>* node = graph->addReturnVertex(newVertexInfo);
        for(auto inc:incoming){
            inc.first->addEdge(node,inc.second);
        }
        for(auto a:adj){
            node->addEdge(a.first,a.second);
        }
        id = 1;
    }
    if(stream.str().empty()){
        cout << "There is no city affected by the removal of any station" << endl;
    }else{
        cout << stream.str();
    }
}



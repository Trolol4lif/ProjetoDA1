#include <wsman.h>
#include "Data.h"
#include "data_structures/Graph.h"

Data::Data() {
    Graph<int> waterG = Graph<int>();
    read_cities();
    read_pipes();
    read_stations();
    read_reservoir();
}

void Data::read_cities() {

}

void Data::read_pipes() {

}

void Data::read_reservoir() {

}

void Data::read_stations() {

}
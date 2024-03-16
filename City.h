#ifndef PROJETODA1_CITY_H
#define PROJETODA1_CITY_H

#include "NodeData.h"

class City : public NodeData{
private:
    string name;
    double demand; // m3/sec
    int population;
public:
    City(const string& name,const int& id,const string& delivery_code,const double& demand,const int& population);
    const string &getName() const;
    void setName(const string &name);
    double getDemand() const;
    void setDemand(int demand);
    int getPopulation() const;
    void setPopulation(int population);
    bool operator==(const City& other) const;
};

#endif //PROJETODA1_CITY_H

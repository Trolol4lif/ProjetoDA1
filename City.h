#ifndef PROJETODA1_CITY_H
#define PROJETODA1_CITY_H


using namespace std;
#include <string>

class City {
private:
    string name;
    int id;
    string delivery_code;
    int demand; // m3/sec
    int population;
public:
    City(const string& name,const string& delivery_code,const int& demand,const int& population);
    const string &getName() const;
    void setName(const string &name);
    int getId() const;
    void setId(int id);
    const string &getDeliveryCode() const;
    void setDeliveryCode(const string &deliveryCode);
    int getDemand() const;
    void setDemand(int demand);
    int getPopulation() const;
    void setPopulation(int population);
};

#endif //PROJETODA1_CITY_H

#ifndef STATION_H
#define STATION_H


#include <iostream>
#include <string>
using namespace std;

class Station{
private:
    int id;
    string code;
public:
    Station();
    Station(int id, string code);
    int getId() const;
    string getCode() const;
    void setId(int id);
    void setCode(const string &code);
};


#endif

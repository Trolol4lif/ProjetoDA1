#ifndef PROJETODA1_RESERVOIR_H
#define PROJETODA1_RESERVOIR_H


#include "NodeData.h"

class Reservoir: public NodeData{
    private:
        string name;
        string municipality;
        int id;
        string code; // water reservoir code
        int maxDelivery; // m3/sec
    public:
        Reservoir(const string &name, const string &municipality,const int &id, const string &code,const int& maxDelivery);
        const string &getName() const;
        void setName(const string &name);
        const string &getMunicipality() const;
        void setMunicipality(const string &municipality);
        int getMaxDelivery() const;
        void setMaxDelivery(int maxDelivery);
        bool operator==(const Reservoir& other) const;
};


#endif //PROJETODA1_RESERVOIR_H

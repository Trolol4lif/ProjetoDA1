#ifndef PROJETODA1_RESERVOIR_H
#define PROJETODA1_RESERVOIR_H


using namespace std;
#include "string"

class Reservoir {
    private:
        string name;
        string municipality;
        int id;
        string code; // water reservoir code
        int maxDelivery; // m3/sec
    public:
        Reservoir(const string &name, const string &municipality, const string &code, int maxDelivery);
        const string &getName() const;
        void setName(const string &name);
        const string &getMunicipality() const;
        void setMunicipality(const string &municipality);
        int getId() const;
        void setId(int id);
        const string &getCode() const;
        void setCode(const string &code);
        int getMaxDelivery() const;
        void setMaxDelivery(int maxDelivery);
};


#endif //PROJETODA1_RESERVOIR_H

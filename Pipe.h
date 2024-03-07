#ifndef PROJETODA1_PIPE_H
#define PROJETODA1_PIPE_H
using namespace std;
#include "string"

class Pipe {
    private:
        string servicePointA;
        string servicePointB;
        int capacity;
        bool direction; // 1 - unidirectional and 0 - bidirectional
public:
    Pipe(const string &servicePointA, const string &servicePointB, int capacity, bool direction);
    const string &getServicePointA() const;
    void setServicePointA(const string &servicePointA);
    const string &getServicePointB() const;
    void setServicePointB(const string &servicePointB);
    int getCapacity() const;
    void setCapacity(int capacity);
    bool isDirection() const;
    void setDirection(bool direction);
};


#endif //PROJETODA1_PIPE_H

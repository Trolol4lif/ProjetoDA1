#ifndef PROJETODA1_NODEDATA_H
#define PROJETODA1_NODEDATA_H


using namespace std;
#include "string"
enum nodeType{
    RESERVOIR,
    STATION,
    CITY,
};
class NodeData {
    public:
        NodeData(int id,const string& code,nodeType type);
        nodeType getType() const;
        int getId() const;
        void setId(int id);
        const string &getCode() const;
        void setCode(const string &code);
        void setType(nodeType type);
        bool operator==(const NodeData& other) const;
    protected:
        int id;
        string code;
        nodeType type;
};


#endif //PROJETODA1_NODEDATA_H

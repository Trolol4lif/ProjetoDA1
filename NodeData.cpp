#include "NodeData.h"

NodeData::NodeData(int id,const string& code,nodeType type){
    this->id = id;
    this->type = type;
    this->code = code;
}

nodeType NodeData::getType() const {
    return type;
}

int NodeData::getId() const {
    return id;
}

void NodeData::setId(int id) {
    NodeData::id = id;
}

const string &NodeData::getCode() const {
    return code;
}

void NodeData::setCode(const string &code) {
    NodeData::code = code;
}

void NodeData::setType(nodeType type) {
    NodeData::type = type;
}

bool NodeData::operator==(const NodeData& other) const{
    return other.getType() == type && other.code == code;
}
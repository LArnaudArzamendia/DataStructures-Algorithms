#include "poscode.hpp"

Poscode::Poscode():data(""){}
Poscode::Poscode(std::string _data) : data(_data){}
char Poscode::getValue(size_t i) const{
    return data[i];
}

const std::string &Poscode::getData() const{
    return data;
}
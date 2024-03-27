#include <iostream>

#include "attribute.hpp"

Attribute::Attribute() {}

Attribute::Attribute(string s) : str(s) {}

vector<Attribute*> Attribute::var_list = vector<Attribute*>(0);

void Attribute::addVar(Attribute* attr) {
    var_list.push_back(attr);
}

Attribute Attribute::getVar(string s) {
    for (auto attr : var_list) if (attr->str == s) return *attr;
    return Attribute(s);
}

Type castType(Type t1,Type t2) {
    return (t1 > t2 ? t1 : t2);
}

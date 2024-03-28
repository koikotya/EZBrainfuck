#include <string>
#include <vector>
#include <iostream>

#include "attribute.hpp"

using std::string;
using std::vector;

Attribute::Attribute() {}

Attribute::Attribute(string s) : str(s) {}

int Attribute::size() {
    return idegit+fdegit+(sign ? 1 : 0);
}

vector<Attribute*> Attribute::var_list = vector<Attribute*>(0);
int Attribute::used_memory[2] = {0,4};

void Attribute::addVar(Attribute* attr) {
    var_list.push_back(attr);
}

Attribute Attribute::getVar(string s) {
    for (auto attr : var_list) if (attr->str == s) return *attr;
    return Attribute(s);
}

void Attribute::allocateVar(Attribute *v,int type) {
    int id = (used_memory[0] > used_memory[1] ? 0 : 1);
    if (type == 0) {
    } else if (type == 1) {
        id = (1^id);
    } else {
    }

    v->address = used_memory[id];
    addVar(v);
    used_memory[id] += v->size()*INTERVAL;
}

void Attribute::deallocVar(Attribute *v) {
    if (var_list.back()->address%8 == 0) {
        used_memory[0] = var_list.back()->address;
    } else {
        used_memory[1] = var_list.back()->address;
    }
    free(v);
}

int getIndex(Operation op) {
    int res = 0;
    if (op == PLUS_OP) res = 2;
    else if (op == MINUS_OP) res = 2;
    else if (op == TIMES_OP) res = 5;
    else if (op == DIVIDE_OP) res = 1;
    else if (op == MOD_OP) res = 5;
    else if (op == EQUAL_COND) res = 3;
    else if (op == NOTEQUAL_COND) res = 5;
    else if (op == LESS_COND) res = 1;
    else if (op == GREATEREQUAL_COND) res = 2;
    else if (op == NOT_OP) res = 1;
    else ;
    return res;
}

Type castType(Type t1,Type t2) {
    return (t1 > t2 ? t1 : t2);
}

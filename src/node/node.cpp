#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

#include "node.hpp"

using std::ostream;
using std::string;

Node::Node() {}

Node::Node(char s[]) {
    str_ = string(s);
}

Node::Node(std::initializer_list<Node*> list) : childs_(list.begin(),list.end()) {
}

ostream& operator<<(ostream& os,const Node& node) {
    node.print(os);
    return os;
}

ostream& Node::print(ostream& os,int i) const {
    if (str_.size() > 0) os << str_ << " ";
    if (!childs_.empty()) {
        os << "{\n";
        for (auto c : childs_) c->print(os,i+1);
        os << string(4*i,' ') << "}";
    }
    os << "\n";
    return os;
}

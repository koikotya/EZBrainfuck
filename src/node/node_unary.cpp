#include <iostream>
#include <string>

#include "node.hpp"
#include "node_unary.hpp"

using std::ostream;
using std::string;

ostream& UnaryPlus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "UnaryPlus ";
    return Node::print(os,i);
}

ostream& UnaryMinus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "UnaryMinus ";
    return Node::print(os,i);
}

#include <iostream>
#include <string>

#include "node.hpp"
#include "node_literal.hpp"

using std::ostream;
using std::string;

ostream& IntNumber::print(ostream& os,int i) const {
    os << string(4*i,' ') << "IntNumber ";
    return Node::print(os,i);
}
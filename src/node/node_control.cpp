#include <iostream>
#include <string>

#include "node.hpp"
#include "node_control.hpp"

using std::ostream;
using std::string;

ostream& If::print(ostream& os,int i) const {
    os << string(4*i,' ') << "If ";
    return Node::print(os,i);
}

ostream& While::print(ostream& os,int i) const {
    os << string(4*i,' ') << "While ";
    return Node::print(os,i);
}

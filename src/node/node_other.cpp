#include <iostream>
#include <string>

#include "node.hpp"
#include "node_other.hpp"

using std::ostream;
using std::string;

ostream& Root::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Root ";
    return Node::print(os,i);
}

ostream& Ident::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Ident ";
    return Node::print(os,i);
}

ostream& Statements::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Statements ";
    return Node::print(os,i);
}

ostream& Assign::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Assign ";
    return Node::print(os,i);
}

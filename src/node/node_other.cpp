#include <iostream>
#include <string>

#include "attribute.hpp"
#include "node.hpp"
#include "node_other.hpp"

using std::ostream;
using std::string;


ostream& Root::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Root ";
    return Node::print(os,i);
}

Ident::Ident(char s[]) : Node(s) {
    attr_ = Attribute::getVar(attr_.str);
}

ostream& Ident::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Ident ";
    os << attr_.str << " ";
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

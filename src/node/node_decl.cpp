#include <iostream>
#include <string>

#include "node.hpp"
#include "node_decl.hpp"

using std::ostream;
using std::string;

ostream& DeclUint::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclUint ";
    return Node::print(os,i);
}

ostream& DeclInt::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclInt ";
    return Node::print(os,i);
}

ostream& DeclFixed::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclFixed ";
    return Node::print(os,i);
}

ostream& DeclChar::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclChar ";
    return Node::print(os,i);
}

ostream& DeclBool::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclBool ";
    return Node::print(os,i);
}

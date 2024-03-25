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

#include <iostream>
#include <string>

#include "node.hpp"
#include "node_io.hpp"

using std::ostream;
using std::string;

ostream& Scan::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Scan ";
    return Node::print(os,i);
}

ostream& PrintNode::print(ostream& os,int i) const {
    os << string(4*i,' ') << "PrintNode ";
    return Node::print(os,i);
}

ostream& PrintStr::print(ostream& os,int i) const {
    os << string(4*i,' ') << "PrintStr ";
    return Node::print(os,i);
}

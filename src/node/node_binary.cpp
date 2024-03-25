#include <iostream>
#include <string>

#include "node.hpp"
#include "node_binary.hpp"

using std::ostream;
using std::string;

ostream& Plus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Plus ";
    return Node::print(os,i);
}

ostream& Minus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Minus ";
    return Node::print(os,i);
}

ostream& Times::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Times ";
    return Node::print(os,i);
}

ostream& Divide::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Divide ";
    return Node::print(os,i);
}

ostream& Mod::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Mod ";
    return Node::print(os,i);
}

ostream& Equal::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Equal ";
    return Node::print(os,i);
}

ostream& NotEqual::print(ostream& os,int i) const {
    os << string(4*i,' ') << "NotEqual ";
    return Node::print(os,i);
}

ostream& Less::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Less ";
    return Node::print(os,i);
}

ostream& GreaterEqual::print(ostream& os,int i) const {
    os << string(4*i,' ') << "GreaterEqual ";
    return Node::print(os,i);
}

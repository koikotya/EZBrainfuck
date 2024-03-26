#include <iostream>
#include <string>

#include "node.hpp"
#include "node_control.hpp"

using std::ostream;
using std::string;

Control::Control(initializer_list<Node*> list) : Node(list) {
    attr_.idegit = 0;
    attr_.fdegit = 0;
    attr_.sign = true;
}

ostream& If::print(ostream& os,int i) const {
    os << string(4*i,' ') << "If ";
    return Node::print(os,i);
}

ostream& IfElse::print(ostream& os,int i) const {
    os << string(4*i,' ') << "IfElse ";
    return Node::print(os,i);
}

ostream& While::print(ostream& os,int i) const {
    os << string(4*i,' ') << "While ";
    return Node::print(os,i);
}

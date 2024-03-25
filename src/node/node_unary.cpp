#include <iostream>
#include <string>

#include "attribute.hpp"
#include "node.hpp"
#include "node_unary.hpp"

using std::ostream;
using std::string;

Unary::Unary(std::initializer_list<Node*> list) : Node(list) {
    attr_ = childs_[0]->attr_;
    attr_.sign = true;
    if (attr_.type == UINT_TYPE) attr_.type = INT_TYPE;
}

UnaryPlus::UnaryPlus(std::initializer_list<Node*> list) : Unary(list) {
}

ostream& UnaryPlus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "UnaryPlus ";
    return Node::print(os,i);
}

UnaryMinus::UnaryMinus(std::initializer_list<Node*> list) : Unary(list) {
    attr_.negative ^= true;
}

ostream& UnaryMinus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "UnaryMinus ";
    return Node::print(os,i);
}

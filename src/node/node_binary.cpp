#include <iostream>
#include <string>

#include "attribute.hpp"
#include "node.hpp"
#include "node_binary.hpp"

using std::ostream;
using std::string;
using std::max;

Binary::Binary(initializer_list<Node*> list) : Node(list) {
    const Attribute& attr0 = childs_[0]->attr_;
    const Attribute& attr1 = childs_[1]->attr_;
    attr_.negative = false;
    attr_.type = castType(attr0.type,attr1.type);
    attr_.idegit = max(attr0.idegit,attr1.idegit);
    attr_.fdegit = max(attr0.fdegit,attr1.fdegit);
    attr_.sign = (attr0.sign|attr1.sign);
}

Plus::Plus(initializer_list<Node*> list) : Binary(list) {
    if (attr_.type != CHAR_TYPE && (childs_[0]->attr_.type == CHAR_TYPE || childs_[1]->attr_.type == CHAR_TYPE)) {
        attr_.idegit = max(attr_.idegit,3);
    }
}

ostream& Plus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Plus ";
    return Node::print(os,i);
}

Minus::Minus(initializer_list<Node*> list) : Binary(list) {
    if (attr_.type != CHAR_TYPE && (childs_[0]->attr_.type == CHAR_TYPE || childs_[1]->attr_.type == CHAR_TYPE)) {
        attr_.idegit = max(attr_.idegit,3);
    }
}

ostream& Minus::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Minus ";
    return Node::print(os,i);
}

// Times::Times(initializer_list<Node*> list) : Binary(list) {}

ostream& Times::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Times ";
    return Node::print(os,i);
}

// Divide::Divide(initializer_list<Node*> list) : Binary(list) {}

ostream& Divide::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Divide ";
    return Node::print(os,i);
}

// Mod::Mod(initializer_list<Node*> list) : Binary(list) {}

ostream& Mod::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Mod ";
    return Node::print(os,i);
}

// Equal::Equal(initializer_list<Node*> list) : Binary(list) {}

ostream& Equal::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Equal ";
    return Node::print(os,i);
}

// NotEqual::NotEqual(initializer_list<Node*> list) : Binary(list) {}

ostream& NotEqual::print(ostream& os,int i) const {
    os << string(4*i,' ') << "NotEqual ";
    return Node::print(os,i);
}

// Less::Less(initializer_list<Node*> list) : Binary(list) {}

ostream& Less::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Less ";
    return Node::print(os,i);
}

// GreaterEqual::GreaterEqual(initializer_list<Node*> list) : Binary(list) {}

ostream& GreaterEqual::print(ostream& os,int i) const {
    os << string(4*i,' ') << "GreaterEqual ";
    return Node::print(os,i);
}

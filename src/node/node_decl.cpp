#include <iostream>
#include <string>

#include "attribute.hpp"
#include "node.hpp"
#include "node_decl.hpp"

using std::ostream;
using std::string;
using std::initializer_list;
using std::stoi;

Decl::Decl(initializer_list<Node*> list) : Node(list) {
    attr_.op = NOT_OP;
    attr_.negative = false;
    attr_.str = childs_[0]->attr_.str;
    delete childs_[0];
    Attribute::addVar(&attr_);
}

DeclUint::DeclUint(initializer_list<Node*> list) : Decl(list) {
    attr_.type = UINT_TYPE;
    attr_.idegit = stoi(childs_[1]->attr_.str);
    attr_.fdegit = 0;
    attr_.sign = false;
    delete childs_[1];
    childs_.clear();
}

ostream& DeclUint::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclUint ";
    os << attr_.str << "(" << attr_.idegit << ") ";
    return Node::print(os,i);
}

DeclInt::DeclInt(initializer_list<Node*> list) : Decl(list) {
    attr_.type = INT_TYPE;
    attr_.idegit = stoi(childs_[1]->attr_.str);
    attr_.fdegit = 0;
    attr_.sign = true;
    delete childs_[1];
    childs_.clear();
}

ostream& DeclInt::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclInt ";
    os << attr_.str << "(" << attr_.idegit << ") ";
    return Node::print(os,i);
}

DeclFixed::DeclFixed(initializer_list<Node*> list) : Decl(list) {
    attr_.type = FIXED_TYPE;
    attr_.idegit = stoi(childs_[1]->attr_.str);
    attr_.fdegit = stoi(childs_[2]->attr_.str);
    attr_.sign = true;
    delete childs_[1];
    delete childs_[2];
    childs_.clear();
}

ostream& DeclFixed::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclFixed ";
    os << attr_.str << "(" << attr_.idegit << ", " << attr_.fdegit << ") ";
    return Node::print(os,i);
}

DeclChar::DeclChar(initializer_list<Node*> list) : Decl(list) {
    attr_.type = CHAR_TYPE;
    attr_.idegit = 1;
    attr_.fdegit = 0;
    attr_.sign = false;
    childs_.clear();
}

ostream& DeclChar::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclChar ";
    os << attr_.str << " ";
    return Node::print(os,i);
}

DeclBool::DeclBool(initializer_list<Node*> list) : Decl(list) {
    attr_.type = BOOL_TYPE;
    attr_.idegit = 1;
    attr_.fdegit = 0;
    attr_.sign = false;
    childs_.clear();
}

ostream& DeclBool::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DeclBool ";
    os << attr_.str << " ";
    return Node::print(os,i);
}

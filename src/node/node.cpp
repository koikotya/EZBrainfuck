#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>

#include "node.hpp"

using std::ostream;
using std::string;

Node::Node() {}

Node::Node(char s[]) {
    str_ = string(s);
}

Node::Node(std::initializer_list<Node*> list) : childs_(list.begin(),list.end()){
}

ostream& Node::printNode(ostream& os,string class_name,int i) const {
    os << string(4*i,' ') << class_name << " ";
    if (str_.size() > 0) os << str_ << " ";
    if (!childs_.empty()) {
        os << "{\n";
        for (auto c : childs_) c->print(os,i+1);
        os << string(4*i,' ') << "}";
    }
    os << "\n";
    return os;
}

ostream& operator<<(ostream& os,const Node& node) {node.print(os);return os;}

ostream& Node::print(ostream& os,int i) const {
    return printNode(os,"Node",i);
}

ostream& Root::print(ostream& os,int i) const {
    return printNode(os,"Root",i);
}

ostream& Ident::print(ostream& os,int i) const {
    return printNode(os,"Ident",i);
}

ostream& Statements::print(ostream& os,int i) const {
    return printNode(os,"Statements",i);
}

ostream& DeclUint::print(ostream& os,int i) const {
    return printNode(os,"DeclUint",i);
}

ostream& IntNumber::print(ostream& os,int i) const {
    return printNode(os,"IntNumber",i);
}
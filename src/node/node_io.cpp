#include <iostream>
#include <string>

#include "attribute.hpp"
#include "node.hpp"
#include "node_io.hpp"

using std::ostream;
using std::string;
using std::initializer_list;

Scan::Scan(initializer_list<Node*> list) : IO(list) {
    attr_.str = childs_[0]->attr_.str;
    delete childs_[0];
    childs_.clear();
}

ostream& Scan::print(ostream& os,int i) const {
    os << string(4*i,' ') << "Scan ";
    os << attr_.str << " ";
    return Node::print(os,i);
}

// PrintNode::PrintNode(initializer_list<Node*> list) : IO(list) {}

ostream& PrintNode::print(ostream& os,int i) const {
    os << string(4*i,' ') << "PrintNode ";
    return Node::print(os,i);
}

PrintLiteral::PrintLiteral(initializer_list<Node*> list) : IO(list) {
    attr_ = childs_[0]->attr_;
    delete childs_[0];
    childs_.clear();
}

ostream& PrintLiteral::print(ostream& os,int i) const {
    os << string(4*i,' ') << "PrintLiteral ";
    os << attr_.str << " ";
    return Node::print(os,i);
}

PrintStr::PrintStr(initializer_list<Node*> list) : IO(list) {
    attr_.str = childs_[0]->attr_.str;
    delete childs_[0];
    childs_.clear();
}

ostream& PrintStr::print(ostream& os,int i) const {
    os << string(4*i,' ') << "PrintStr ";
    os << attr_.str << " ";
    return Node::print(os,i);
}

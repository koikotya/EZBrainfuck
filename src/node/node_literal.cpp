#include <iostream>
#include <string>

#include "attribute.hpp"
#include "node.hpp"
#include "node_literal.hpp"

using std::ostream;
using std::string;

IntNumber::IntNumber(char s[]) : Literal(s) {
    attr_.type = UINT_TYPE;
    attr_.op = INT_LITERAL;
    attr_.idegit = attr_.str.size();
    attr_.fdegit = 0;
    attr_.sign = false;
}

ostream& IntNumber::print(ostream& os,int i) const {
    os << string(4*i,' ') << "IntNumber ";
    if (attr_.str.size() > 0) os << attr_.str << " ";
    return Node::print(os,i);
}

DecimalNumber::DecimalNumber(char s[]) : Literal(s) {
    attr_.type = FIXED_TYPE;
    attr_.op = DECIMAL_LITERAL;
    attr_.idegit = 0;
    for (int i = 0;i < attr_.str.size();++i) {
        if (attr_.str[i] == '.') break;
        attr_.idegit++;
    }
    attr_.fdegit = attr_.str.size()-1-attr_.idegit;
    attr_.sign = true;
}

ostream& DecimalNumber::print(ostream& os,int i) const {
    os << string(4*i,' ') << "DecimalNumber ";
    if (attr_.str.size() > 0) os << attr_.str << " ";
    return Node::print(os,i);
}

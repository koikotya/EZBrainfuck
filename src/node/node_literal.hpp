#ifndef INCLUDED_LITERAL
#define INCLUDED_LITERAL

#include "attribute.hpp"
#include "node.hpp"


class Literal : public Node {
public:
    using Node::Node;
};

class IntNumber : public Literal {
public:
    using Literal::Literal;

    IntNumber(char s[]) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class DecimalNumber : public Literal {
public:
    using Literal::Literal;

    DecimalNumber(char s[]) ;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

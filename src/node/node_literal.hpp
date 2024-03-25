#ifndef INCLUDED_LITERAL
#define INCLUDED_LITERAL

#include "node.hpp"

class Literal : public Node {
    using Node::Node;
};

class IntNumber : public Literal {
    using Literal::Literal;

    ostream& print(ostream& os,int i = 0) const override;
};


#endif
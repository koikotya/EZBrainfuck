#ifndef INCLUDED_UNARY
#define INCLUDED_UNARY

#include "node.hpp"

class Unary : public Node {
    using Node::Node;
};

class UnaryPlus : public Unary {
    using Unary::Unary;

    ostream& print(ostream& os,int i = 0) const override;
};

class UnaryMinus : public Unary {
    using Unary::Unary;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

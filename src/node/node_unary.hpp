#ifndef INCLUDED_UNARY
#define INCLUDED_UNARY

#include "attribute.hpp"
#include "node.hpp"

using std::initializer_list;

class Unary : public Node {
public:
    using Node::Node;

    Unary(initializer_list<Node*> list) ;
};

class UnaryPlus : public Unary {
public:
    using Unary::Unary;

    UnaryPlus(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class UnaryMinus : public Unary {
public:
    using Unary::Unary;

    UnaryMinus(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

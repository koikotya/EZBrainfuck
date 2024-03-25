#ifndef INCLUDED_DECL
#define INCLUDED_DECL

#include "node.hpp"

class Decl : public Node {
    using Node::Node;
};

class DeclUint : public Decl {
    using Decl::Decl;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif
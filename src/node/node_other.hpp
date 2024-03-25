#ifndef INCLUDED_OTHER
#define INCLUDED_OTHER

#include "node.hpp"

class Root : public Node {
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

class Ident : public Node {
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

class Statements : public Node {
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

class Assign : public Node {
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

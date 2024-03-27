#ifndef INCLUDED_OTHER
#define INCLUDED_OTHER

#include "attribute.hpp"
#include "node.hpp"

class Root : public Node {
public:
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

class Ident : public Node {
public:
    using Node::Node;

    Ident(char s[]) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Statements : public Node {
public:
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

class Assign : public Node {
public:
    using Node::Node;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

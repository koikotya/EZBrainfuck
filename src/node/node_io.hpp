#ifndef INCLUDED_IO
#define INCLUDED_IO

#include "attribute.hpp"
#include "node.hpp"

using std::initializer_list;

class IO : public Node {
public:
    using Node::Node;
};

class Scan : public IO {
public:
    using IO::IO;

    Scan(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class PrintNode : public IO {
public:
    using IO::IO;

    // PrintNode(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class PrintLiteral : public IO {
public:
    using IO::IO;

    PrintLiteral(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class PrintStr : public IO {
public:
    using IO::IO;

    PrintStr(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

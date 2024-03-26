#ifndef INCLUDED_DECL
#define INCLUDED_DECL

#include "attribute.hpp"
#include "node.hpp"

using std::initializer_list;

class Decl : public Node {
public:
    using Node::Node;

    Decl(initializer_list<Node*> list) ;
};

class DeclUint : public Decl {
public:
    using Decl::Decl;

    DeclUint(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class DeclInt : public Decl {
public:
    using Decl::Decl;

    DeclInt(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class DeclFixed : public Decl {
public:
    using Decl::Decl;

    DeclFixed(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class DeclChar : public Decl {
public:
    using Decl::Decl;

    DeclChar(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class DeclBool : public Decl {
public:
    using Decl::Decl;

    DeclBool(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

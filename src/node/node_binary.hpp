#ifndef INCLUDED_BINARY
#define INCLUDED_BINARY

#include "attribute.hpp"
#include "node.hpp"

using std::initializer_list;

class Binary : public Node {
public:
    using Node::Node;

    Binary(initializer_list<Node*> list) ;
};

class Plus : public Binary {
public:
    using Binary::Binary;

    Plus(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Minus: public Binary {
public:
    using Binary::Binary;

    Minus(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Times : public Binary {
public:
    using Binary::Binary;

    // Times(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Divide: public Binary {
public:
    using Binary::Binary;

    // Divide(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Mod : public Binary {
public:
    using Binary::Binary;

    // Mod(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Equal : public Binary {
public:
    using Binary::Binary;

    // Equal(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class NotEqual: public Binary {
public:
    using Binary::Binary;

    // NotEqual(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class Less : public Binary {
public:
    using Binary::Binary;

    // Less(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

class GreaterEqual : public Binary {
public:
    using Binary::Binary;

    // GreaterEqual(initializer_list<Node*> list) ;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

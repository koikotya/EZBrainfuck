#ifndef INCLUDED_BINARY
#define INCLUDED_BINARY

#include "node.hpp"

class Binary : public Node {
    using Node::Node;
};

class Plus : public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class Minus: public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class Times : public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class Divide: public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class Mod : public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class Equal : public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class NotEqual: public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class Less : public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

class GreaterEqual : public Binary {
    using Binary::Binary;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

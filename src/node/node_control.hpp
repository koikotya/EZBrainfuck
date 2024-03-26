#ifndef INCLUDED_CONTROL
#define INCLUDED_CONTROL

#include "attribute.hpp"
#include "node.hpp"

using std::initializer_list;

class Control : public Node {
public:
    using Node::Node;

    Control(initializer_list<Node*> list) ;
};

class If : public Control {
public:
    using Control::Control;

    ostream& print(ostream& os,int i = 0) const override;
};

class IfElse : public Control {
public:
    using Control::Control;

    ostream& print(ostream& os,int i = 0) const override;
};

class While : public Control {
public:
    using Control::Control;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

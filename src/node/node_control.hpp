#ifndef INCLUDED_CONTROL
#define INCLUDED_CONTROL

#include "node.hpp"

class Control : public Node {
    using Node::Node;
};

class If : public Control {
    using Control::Control;

    ostream& print(ostream& os,int i = 0) const override;
};

class While : public Control {
    using Control::Control;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

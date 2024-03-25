#ifndef INCLUDED_IO
#define INCLUDED_IO

#include "node.hpp"

class IO : public Node {
    using Node::Node;
};

class Scan : public IO {
    using IO::IO;

    ostream& print(ostream& os,int i = 0) const override;
};

class PrintNode : public IO {
    using IO::IO;

    ostream& print(ostream& os,int i = 0) const override;
};

class PrintStr : public IO {
    using IO::IO;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif

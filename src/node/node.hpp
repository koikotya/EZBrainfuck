#ifndef INCLUDED_NODE
#define INCLUDED_NODE

#include<vector>
#include<string>
#include<initializer_list>

using std::vector;
using std::ostream;
using std::string;
/*
NODE_GEN
    UNARY 
        UNARY_PLUS
        UNARY_MINUS
    BINARY
        PLUS
        MINUS
        TIMES
        DIVIDE
        MOD
        EQUAL
        NOTEQUAL
        LESS
        GREATEREQUAL
    DECL
        DECL_UINT
        DECL_INT
        DECL_FIXED
        DECL_BOOL
        DECL_CHAR
    LITERAL
        INTNUMBER
        DECIMALNUMBER
    CONTROL
        IF
        WHILE
    IO
        SCAN
        PRINT_NODE
        PRINT_LITERAL
        PRINT_STR
    ASSIGN
    IDENT
    STATEMENTS
    ROOT
*/

class Node {
public:
    vector<Node*> childs_;
    string str_;

    Node() ;
    Node(char s[]) ;
    Node(std::initializer_list<Node*> list) ;

    virtual ostream& print(ostream& os,int i = 0) const;
    ostream& printNode(ostream& os,string class_name,int i = 0) const;

    friend ostream& operator<<(ostream& os,const Node& node) ;
};

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

class Decl : public Node {
    using Node::Node;
};

class DeclUint : public Decl {
    using Decl::Decl;

    ostream& print(ostream& os,int i = 0) const override;
};

class Literal : public Node {
    using Node::Node;
};

class IntNumber : public Literal {
    using Literal::Literal;

    ostream& print(ostream& os,int i = 0) const override;
};

#endif


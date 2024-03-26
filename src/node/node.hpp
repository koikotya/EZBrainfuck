#ifndef INCLUDED_NODE
#define INCLUDED_NODE

#include <vector>
#include <string>
#include <initializer_list>

#include "attribute.hpp"

using std::vector;
using std::ostream;
using std::string;
/*
NODE
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
    Attribute attr_;

    Node() ;
    Node(char s[]) ;
    Node(std::initializer_list<Node*> list) ;

    virtual ostream& print(ostream& os,int i = 0) const;

    friend ostream& operator<<(ostream& os,const Node& node) ;
};

#endif

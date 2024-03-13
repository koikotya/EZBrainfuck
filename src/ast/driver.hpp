#ifndef INCLUDED_DRIVER
#define INCLUDED_DRIVER

#include<stdio.h>

#include "node.hpp"
#include "parser.hpp"

#define YY_DECL yy::parser::symbol_type yylex (Driver& drv)
YY_DECL;

class Driver {
private:
public:
    Driver();
    Driver(char* f);

    Node* root_;
    char* file_name_;
    FILE file_;

    void parse();
    void fileRead();
    void fileClose();
};


#endif
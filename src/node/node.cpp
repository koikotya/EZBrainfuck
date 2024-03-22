#include <stdio.h>
#include <string.h>

#include "node.hpp"

const char *Ntype_str[] = {
    "UNARY_PLUS_AST",
    "UNARY_MINUS_AST",
    "PLUS_AST",
    "MINUS_AST",
    "TIMES_AST",
    "DIVIDE_AST",
    "MOD_AST",
    "ASSIGN_AST",
    "INTNUMBER_AST",
    "DECIMALNUMBER_AST",
    "IF_AST",
    "WHILE_AST",
    "ARGUMENTS_AST",
    "UINT_AST",
    "INT_AST",
    "FIXED_AST",
    "BOOL_AST",
    "CHAR_AST",
    "STR_AST",
    "EQUAL_AST",
    "NOTEQUAL_AST",
    "LESS_AST",
    "GREATEREQUAL_AST",
    "IDENT_AST",
    "SCAN_AST",
    "PRINT_AST",
    "MAIN_AST",
    "STATEMENTS_AST",
};

Node::Node() {}

Node::Node(Ntype t,char s[]) {
    type = t;
    str = std::string(s);
}

Node::Node(Ntype t, std::initializer_list<Node*> list) : type(t), childs(list.begin(),list.end()){
}

void Node::print(int i) const {
    for (int j = 0;j < i;++j) printf("  ");
    printf("%s ",Ntype_str[type]);
    if (str.size() > 0) printf("%s ",str.c_str());
    if (!childs.empty()) {
        printf("{\n");
        for (auto c : childs) c->print(i+1);
        for (int j = 0;j < i;++j) printf("  ");
        printf("}");
    }
    printf("\n");
}
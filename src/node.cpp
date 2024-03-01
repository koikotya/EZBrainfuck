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
    "IF_ELSE_AST",
    "WHILE_AST",
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
    "DEFAULT"
};

Node::Node() : left(nullptr), right(nullptr) {}

Node::Node(Ntype t,char s[]) {
    type = t;
    left = nullptr;
    right = nullptr;
    strcpy(str,s);
}

Node::Node(Ntype t, Node* p1) {
    type = t;
    left = p1;
    right = nullptr;
}

Node::Node(Ntype t, Node* p1, Node* p2) {
    type = t;
    left = p1;
    right = p2;
}

void Node::print(int i) {
    if (type == STATEMENTS_AST) {
        if (left != nullptr) left->print(i);
        if (right != nullptr) right->print(i);
    } else {
        for (int j = 0;j < i;++j) printf("  ");
        printf("%s ",Ntype_str[type]);
        if (strlen(str) > 0) printf("%s ",str);
        if (left != nullptr) {
            printf("{\n");
            left->print(i+1);
            if (right != nullptr) right->print(i+1);
            for (int j = 0;j < i;++j) printf("  ");
            printf("}");
        }
        printf("\n");
    }
}
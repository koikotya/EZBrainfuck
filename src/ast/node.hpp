#ifndef INCLUDED_NODE
#define INCLUDED_NODE

#include<vector>
#include<string>
#include<initializer_list>

enum Ntype {
    UNARY_PLUS_AST = 0,
    UNARY_MINUS_AST,
    PLUS_AST,
    MINUS_AST,
    TIMES_AST,
    DIVIDE_AST,
    MOD_AST,
    ASSIGN_AST,
    INTNUMBER_AST,
    DECIMALNUMBER_AST,
    IF_AST,
    WHILE_AST,
    ARGUMENTS_AST,
    UINT_AST,
    INT_AST,
    FIXED_AST,
    BOOL_AST,
    CHAR_AST,
    STR_AST,
    EQUAL_AST,
    NOTEQUAL_AST,
    LESS_AST,
    GREATEREQUAL_AST,
    IDENT_AST,
    SCAN_AST,
    PRINT_AST,
    MAIN_AST,
    STATEMENTS_AST,
};

struct Node {
    Ntype type;
    std::vector<Node*> childs;
    std::string str;

    Node();
    Node(Ntype t,char s[]);
    Node(Ntype t, std::initializer_list<Node*> list);

    void print(int i = 0) const;
};

#endif


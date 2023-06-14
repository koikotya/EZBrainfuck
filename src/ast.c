#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

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
};

Node* buildStrNode(Ntype t,char s[]) {
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->type = t;
    strcpy(p->str,s);
    return p;
}

Node* buildNode1(Ntype t,Node* p1) {
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->type = t;
    p->n = 1;
    p->list = (Node**)malloc(p->n*sizeof(Node*));
    p->list[0] = p1;
    return p;
}

Node* buildNode2(Ntype t,Node* p1,Node* p2) {
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->type = t;
    p->n = 2;
    p->list = (Node**)malloc(p->n*sizeof(Node*));
    p->list[0] = p1;
    p->list[1] = p2;
    return p;
}

Node* buildNode3(Ntype t,Node* p1,Node* p2,Node* p3) {
    Node *p;
    p = (Node *)malloc(sizeof(Node));
    p->type = t;
    p->n = 3;
    p->list = (Node**)malloc(p->n*sizeof(Node*));
    p->list[0] = p1;
    p->list[1] = p2;
    p->list[2] = p3;
    return p;
}

void printNode(Node *p,int i) {
    for (int j = 0;j < i;++j) printf("  ");
    printf("%s ",Ntype_str[p->type]);
    if (strlen(p->str) > 0) printf("%s ",p->str);
    if (0 < p->n) {
        printf("{\n");
        for (int j = 0;j < p->n;++j) if (p->list[j] != NULL) printNode(p->list[j],i+1);
        for (int j = 0;j < i;++j) printf("  ");
        printf("}");
    }
    printf("\n");
}
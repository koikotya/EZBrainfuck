#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "func.h"

int list_size = 0;
Variable *val_list[100010];

void setMemorySize(Node *p) {
    Variable *res = NULL;
    if (p == NULL) return;
    if (p->type == UNARY_PLUS_AST) {
        setMemorySize(p->list[0]);
        res = (Variable *)malloc(sizeof(Variable));
        *res = *p->list[0]->v;
        res->sign = true;
        if (res->type == UINT_TYPE) res->type = INT_TYPE;
    } else if (p->type == UNARY_MINUS_AST) {
        setMemorySize(p->list[0]);
        res = (Variable *)malloc(sizeof(Variable));
        *res = *p->list[0]->v;
        res->sign = true;
        if (res->type == UINT_TYPE) res->type = INT_TYPE;
        res->negative = (!res->negative);
    } else if (p->type == PLUS_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = PLUS_OP;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        if (res->type != CHAR_TYPE && (v1->type == CHAR_TYPE || v2->type == CHAR_TYPE)) res->idegit = max(res->idegit,3);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == MINUS_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = MINUS_OP;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        if (res->type != CHAR_TYPE && (v1->type == CHAR_TYPE || v2->type == CHAR_TYPE)) res->idegit = max(res->idegit,3);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == TIMES_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = TIMES_OP;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == DIVIDE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = DIVIDE_OP;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == MOD_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = MOD_OP;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == ASSIGN_AST) {
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
    } else if (p->type == INTNUMBER_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = UINT_TYPE;
        res->op = INT_LITERAL;
        res->idegit = strlen(p->str);
        res->fdegit = 0;
        res->sign = false;
        res->ident = p->str;
    } else if (p->type == DECIMALNUMBER_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = FIXED_TYPE;
        res->op = DECIMAL_LITERAL;
        res->idegit = 0;
        for (int i = 0;i < strlen(p->str);++i) {
            if (p->str[i] == '.') break;
            res->idegit++;
        }
        res->fdegit = strlen(p->str)-1-res->idegit;
        res->sign = true;
        res->ident = p->str;
    } else if (p->type == IF_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == IF_ELSE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        setMemorySize(p->list[2]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == WHILE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == UINT_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = UINT_TYPE;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = 0;
        res->sign = false;
        res->negative = false;
        res->ident = p->list[1]->str;
        val_list[list_size++] = res;
    } else if (p->type == INT_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = INT_TYPE;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = 0;
        res->sign = true;
        res->negative = false;
        res->ident = p->list[1]->str;
        val_list[list_size++] = res;
    } else if (p->type == FIXED_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = FIXED_TYPE;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = atoi(p->list[1]->str);
        res->sign = true;
        res->negative = false;
        res->ident = p->list[2]->str;
        val_list[list_size++] = res;
    } else if (p->type == BOOL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = BOOL_TYPE;
        res->op = NOT_OP;
        res->idegit = 1;
        res->fdegit = 0;
        res->sign = false;
        res->negative = false;
        res->ident = p->list[0]->str;
        val_list[list_size++] = res;
    } else if (p->type == CHAR_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = CHAR_TYPE;
        res->op = NOT_OP;
        res->idegit = 1;
        res->fdegit = 0;
        res->sign = false;
        res->negative = false;
        res->ident = p->list[0]->str;
        val_list[list_size++] = res;
    } else if (p->type == STR_AST) {
    } else if (p->type == EQUAL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = EQUAL_COND;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = true;
    } else if (p->type == NOTEQUAL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = NOTEQUAL_COND;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = true;
    } else if (p->type == LESS_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = LESS_COND;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = true;
    } else if (p->type == GREATEREQUAL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = GREATEREQUAL_COND;
        res->negative = false;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = true;
    } else if (p->type == IDENT_AST) {
        // todo:マッチしなかった場合にエラーを表示
        res = (Variable *)malloc(sizeof(Variable));
        for (int i = 0;i < list_size;++i) if (val_list[i]->ident != NULL && strcmp(val_list[i]->ident,p->str) == 0) {
            *res = *val_list[i];
            break;
        }
    } else if (p->type == SCAN_AST) {
        setMemorySize(p->list[0]);
    } else if (p->type == PRINT_AST) {
        setMemorySize(p->list[0]);
    } else if (p->type == MAIN_AST) {
        for (int i = 0;i < p->n;++i) setMemorySize(p->list[i]);
        // for (int i = 0;i < list_size;++i) printf("%d\n",val_list[i]->address);
    } else if (p->type == STATEMENTS_AST) {
        for (int i = 0;i < p->n;++i) setMemorySize(p->list[i]);
    } else {
    }
    p->v = res;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
// #include "generator.h"

int list_size = 0;
Variable *val_list[256];

int used_memory = 0;

int min(int a,int b) {
    return (a < b ? a : b);
}

int max(int a,int b) {
    return (a > b ? a : b);
}

int size(Variable *v) {
    return v->idegit+v->fdegit+(v->sign ? 1 : 0);
}

int getIndex(Operation op) {
    int res = 0;
    if (op == PLUS_TEMP) res = 1;
    else if (op == MINUS_TEMP) res = 1;
    else if (op == TIMES_TEMP) res = 4;
    else if (op == DIVIDE_TEMP) res = 7;
    else if (op == MOD_TEMP) res = 1;
    else if (op == EQUAL_COND) res = 2;
    else if (op == NOTEQUAL_COND) ;
    else if (op == LESS_COND) res = 0;
    else if (op == LESSEQUAL_COND) ;
    else if (op == GREATER_COND) ;
    else if (op == GREATEREQUAL_COND) ;
    else ;
    return res;
}

void freeVariable(Variable *v) {
    list_size--;
    used_memory = val_list[list_size]->location;
    free(v);
}

void setLiteral(Variable *v1,Variable *v2,int index1) {
    if (v1->type == UINT_TYPE) {
        setInteger(v1,index1,v2->ident);
    } else if (v1->type == INT_TYPE) {
        setInteger(v1,index1,v2->ident);
        setSign(v1,index1,v2->negative);
    } else if (v1->type == FIXED_TYPE) {
        setDecimal(v1,index1,v2->ident,v2->op);
        setSign(v1,index1,v2->negative);
    } else if (v1->type == CHAR_TYPE) {
        setChar(v1,index1,v2->ident);
    } else if (v1->type == BOOL_TYPE) {
        setChar(v1,index1,v2->ident);
    } else {
        printf("error setLiteral\n");
    }
}

Typename castType(Typename t1,Typename t2) {
    if (t1 == t2) return t1;
    if ((t1 == UINT_TYPE && t2 == INT_TYPE) || (t1 == INT_TYPE && t2 == UINT_TYPE)) return INT_TYPE;
    if (((t1 == UINT_TYPE || t1 == INT_TYPE) && t2 == FIXED_TYPE) || ((t2 == UINT_TYPE || t2 == INT_TYPE) && t1 == FIXED_TYPE)) return FIXED_TYPE;
    printf("type error\n");
    exit(1);
}

// v2をv1のメモリに書き込む
// v1のどのindexに書き込みかは引数で指定
void setValue(Variable *v1,Variable *v2,int index1) {
    int v2_index = 0;
    if (v2->op == INT_LITERAL || v2->op == DECIMAL_LITERAL) {
        // v2がリテラルの場合
        setLiteral(v1,v2,index1);
    } else if (v2->op == PLUS_TEMP || v2->op == MINUS_TEMP || v2->op == TIMES_TEMP || v2->op == DIVIDE_TEMP || v2->op == MOD_TEMP) {
        // v2が計算中の値の場合
        // v2をv1にムーブ
        v2_index = getIndex(v2->op);
        int fdegit = min(v1->fdegit,v2->fdegit);
        int idegit = min(v1->idegit,v2->idegit);
        move(v1,v2,v1->fdegit-fdegit,v2->fdegit-fdegit,index1,v2_index,fdegit+idegit);
        // 符号の処理
        if ((v1->type == INT_TYPE || v1->type == FIXED_TYPE) && (v2->type == INT_TYPE || v2->type == FIXED_TYPE)) {
            move(v1,v2,size(v1)-1,size(v2)-1,index1,v2_index,1);
        }
        turnSign(v1,index1,v2->negative);
        clear(v2,0,v2_index,size(v2));
        freeVariable(v2);
    } else {
        // v2が変数の場合
        // v2をv1にコピー
        // todo:a = -aでバグる問題を解決
        int fdegit = min(v1->fdegit,v2->fdegit);
        int idegit = min(v1->idegit,v2->idegit);
        copy(v1,v2,v1->fdegit-fdegit,v2->fdegit-fdegit,index1,v2_index,fdegit+idegit,1);
        // 符号の処理
        if ((v1->type == INT_TYPE || v1->type == FIXED_TYPE) && (v2->type == INT_TYPE || v2->type == FIXED_TYPE)) {
            copy(v1,v2,size(v1)-1,size(v2)-1,index1,v2_index,1,1);
        }
        turnSign(v1,index1,v2->negative);
    }
};

// todo:激ヤバ実装なのでどうにかする
// todo:dfsの分割。1回目のdfsで桁数などを取得、2回目のdfsでメモリの位置を確定、コード生成
void dfs1(Node *p) {
    Variable *res = NULL;
    if (p == NULL) return;
    if (p->type == PLUS_AST) {
        if (p->n == 1) {
            dfs1(p->list[0]);
            res = (Variable *)malloc(sizeof(Variable));
            *res = *p->list[0]->v;
            res->sign = true;
        } else {
            res = (Variable *)malloc(sizeof(Variable));
            res->op = PLUS_TEMP;
            res->unit_size = 8;
            res->negative = false;
            dfs1(p->list[0]);
            dfs1(p->list[1]);
            Variable *v1 = p->list[0]->v;
            Variable *v2 = p->list[1]->v;
            res->type = castType(v1->type,v2->type);
            res->idegit = max(v1->idegit,v2->idegit);
            res->fdegit = max(v1->fdegit,v2->fdegit);
            res->sign = (v1->sign|v2->sign);
        }
    } else if (p->type == MINUS_AST) {
        if (p->n == 1) {
            dfs1(p->list[0]);
            res = (Variable *)malloc(sizeof(Variable));
            *res = *p->list[0]->v;
            res->sign = true;
            // res->negative = (!res->negative);
        } else {
            res = (Variable *)malloc(sizeof(Variable));
            res->op = MINUS_TEMP;
            res->unit_size = 8;
            res->negative = false;
            dfs1(p->list[0]);
            dfs1(p->list[1]);
            Variable *v1 = p->list[0]->v;
            Variable *v2 = p->list[1]->v;
            res->type = castType(v1->type,v2->type);
            res->idegit = max(v1->idegit,v2->idegit);
            res->fdegit = max(v1->fdegit,v2->fdegit);
            res->sign = (v1->sign|v2->sign);
        }
    } else if (p->type == TIMES_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = TIMES_TEMP;
        res->unit_size = 8;
        res->negative = false;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == DIVIDE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = DIVIDE_TEMP;
        res->unit_size = 8;
        res->negative = false;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == MOD_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = MOD_TEMP;
        res->unit_size = 8;
        res->negative = false;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = (v1->sign|v2->sign);
    } else if (p->type == ASSIGN_AST) {
        dfs1(p->list[0]);
        dfs1(p->list[1]);
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
        res->unit_size = 2;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == IF_ELSE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->unit_size = 2;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        dfs1(p->list[2]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == WHILE_AST) {
    } else if (p->type == UINT_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = UINT_TYPE;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = 0;
        res->sign = false;
        res->unit_size = 3;
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
        res->unit_size = 3;
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
        res->unit_size = 3;
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
        res->unit_size = 2;
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
        res->unit_size = 2;
        res->negative = false;
        res->ident = p->list[0]->str;
        val_list[list_size++] = res;
    } else if (p->type == STR_AST) {
    } else if (p->type == EQUAL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = EQUAL_COND;
        res->unit_size = 8;
        res->negative = false;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = true;
    } else if (p->type == NOTEQUAL_AST) {
    } else if (p->type == LESS_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = LESS_COND;
        res->unit_size = 8;
        res->negative = false;
        dfs1(p->list[0]);
        dfs1(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        res->type = castType(v1->type,v2->type);
        res->idegit = max(v1->idegit,v2->idegit);
        res->fdegit = max(v1->fdegit,v2->fdegit);
        res->sign = true;
    } else if (p->type == LESSEQUAL_AST) {
    } else if (p->type == GREATER_AST) {
    } else if (p->type == GREATEREQUAL_AST) {
    } else if (p->type == IDENT_AST) {
        // todo:マッチしなかった場合にエラーを表示
        res = (Variable *)malloc(sizeof(Variable));
        for (int i = 0;i < list_size;++i) if (strcmp(val_list[i]->ident,p->str) == 0) {
            *res = *val_list[i];
            break;
        }
    } else if (p->type == SCAN_AST) {
        dfs1(p->list[0]);
    } else if (p->type == PRINT_AST) {
        dfs1(p->list[0]);
    } else if (p->type == MAIN_AST) {
        for (int i = 0;i < p->n;++i) dfs1(p->list[i]);
        // for (int i = 0;i < list_size;++i) printf("%d\n",val_list[i]->location);
    } else if (p->type == STATEMENTS_AST) {
        for (int i = 0;i < p->n;++i) dfs1(p->list[i]);
    } else {
    }
    p->v = res;
}

void dfs2(Node *p) {
    if (p == NULL) return;
    Variable *v0 = p->v;
    if (p->type == PLUS_AST) {
        if (p->n == 1) {
            dfs2(p->list[0]);
        } else {
            v0->location = used_memory;
            v0->unit_size = 8;
            v0->negative = false;
            val_list[list_size++] = v0;
            used_memory += size(v0)*v0->unit_size;
            dfs2(p->list[0]);
            dfs2(p->list[1]);
            Variable *v1 = p->list[0]->v;
            Variable *v2 = p->list[1]->v;
            setValue(v0,v1,1);
            setValue(v0,v2,0);

            movePointer(0,v0->location);
            if (v0->type == UINT_TYPE) {
                add(v0->idegit);
            } else if (v0->type == INT_TYPE) {
                signedAdd(v0->idegit);
            } else if (v0->type == FIXED_TYPE) {
                signedAdd(v0->idegit+v0->fdegit);
            } else if (v0->type == CHAR_TYPE) {
                charAdd();
            } else if (v0->type == BOOL_TYPE) {
                // 定義しない
            } else {
                // error
            }
            movePointer(v0->location,0);
        }
    } else if (p->type == MINUS_AST) {
        if (p->n == 1) {
            dfs2(p->list[0]);
            v0->negative = (!v0->negative);
        } else {
            v0->location = used_memory;
            v0->unit_size = 8;
            v0->negative = false;
            val_list[list_size++] = v0;
            used_memory += size(v0)*v0->unit_size;
            dfs2(p->list[0]);
            dfs2(p->list[1]);
            Variable *v1 = p->list[0]->v;
            Variable *v2 = p->list[1]->v;
            setValue(v0,v1,1);
            setValue(v0,v2,0);

            movePointer(0,v0->location);
            if (v0->type == UINT_TYPE) {
                sub(v0->idegit);
            } else if (v0->type == INT_TYPE) {
                signedSub(v0->idegit);
            } else if (v0->type == FIXED_TYPE) {
                signedSub(v0->idegit+v0->fdegit);
            } else if (v0->type == CHAR_TYPE) {
                charSub();
            } else if (v0->type == BOOL_TYPE) {
                // 定義しない
            } else {
                // error
            }
            movePointer(v0->location,0);
        }
    } else if (p->type == TIMES_AST) {
        v0->idegit = v0->idegit+v0->idegit+v0->fdegit;

        v0->location = used_memory;
        v0->unit_size = 8;
        v0->negative = false;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
        dfs2(p->list[0]);
        dfs2(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,1);
        setValue(v0,v2,0);

        v0->idegit -= v0->fdegit;
        v0->fdegit += v0->fdegit;

        movePointer(0,v0->location);
        if (v0->type == UINT_TYPE) {
            multShort(v0->idegit/2);
        } else if (v0->type == INT_TYPE) {
            signedMultShort(v0->idegit/2);
        } else if (v0->type == FIXED_TYPE) {
            signedMultShort((v0->idegit+v0->fdegit)/2);
        } else if (v0->type == CHAR_TYPE) {
            // 定義しない
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
        movePointer(v0->location,0);
    } else if (p->type == DIVIDE_AST) {
        v0->idegit = v0->idegit+v0->idegit;
        v0->fdegit = v0->fdegit+v0->fdegit;

        v0->location = used_memory;
        v0->unit_size = 8;
        v0->negative = false;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
        dfs2(p->list[0]);
        dfs2(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,1);

        v0->fdegit /= 2;
        v0->idegit += v0->fdegit;
        setValue(v0,v2,0);

        movePointer(0,v0->location);
        if (v0->type == UINT_TYPE) {
            divide(v0->idegit/2);
        } else if (v0->type == INT_TYPE) {
            signedDivide(v0->idegit/2);
        } else if (v0->type == FIXED_TYPE) {
            signedDivide((v0->idegit+v0->fdegit)/2);
        } else if (v0->type == CHAR_TYPE) {
            // 定義しない
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
        movePointer(v0->location,0);
        clear(v0,0,0,size(v0));
        clear(v0,0,1,size(v0));
    } else if (p->type == MOD_AST) {
        v0->idegit = v0->idegit+v0->idegit;
        v0->fdegit = v0->fdegit+v0->fdegit;

        v0->location = used_memory;
        v0->unit_size = 8;
        v0->negative = false;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
        dfs2(p->list[0]);
        dfs2(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,1);

        v0->fdegit /= 2;
        v0->idegit += v0->fdegit;
        setValue(v0,v2,0);

        movePointer(0,v0->location);
        if (v0->type == UINT_TYPE) {
            divide(v0->idegit/2);
        } else if (v0->type == INT_TYPE) {
            signedDivide(v0->idegit/2);
        } else if (v0->type == FIXED_TYPE) {
            signedDivide((v0->idegit+v0->fdegit)/2);
        } else if (v0->type == CHAR_TYPE) {
            // 定義しない
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
        movePointer(v0->location,0);
        clear(v0,0,0,size(v0));
        clear(v0,0,7,size(v0));
    } else if (p->type == ASSIGN_AST) {
        dfs2(p->list[0]);
        dfs2(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        clear(v1,0,0,size(v1));
        setValue(v1,v2,0);
    } else if (p->type == INTNUMBER_AST) {
    } else if (p->type == DECIMALNUMBER_AST) {
    } else if (p->type == IF_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;

        dfs2(p->list[0]);
        Variable *v1 = p->list[0]->v;
        move(v0,v1,0,size(v1)-1,0,getIndex(v1->op),1);
        freeVariable(v1);
        ifBegin(v0);

        dfs2(p->list[1]);

        ifEnd(v0);
        freeVariable(v0);
    } else if (p->type == IF_ELSE_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;

        dfs2(p->list[0]);
        Variable *v1 = p->list[0]->v;
        move(v0,v1,0,size(v1)-1,0,getIndex(v1->op),1);
        freeVariable(v1);
        ifElseBegin(v0);

        dfs2(p->list[1]);

        ifElseMid(v0);

        dfs2(p->list[2]);

        ifElseEnd(v0);
        freeVariable(v0);
    } else if (p->type == WHILE_AST) {
    } else if (p->type == UINT_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
    } else if (p->type == INT_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
    } else if (p->type == FIXED_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
    } else if (p->type == BOOL_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
    } else if (p->type == CHAR_AST) {
        v0->location = used_memory;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
    } else if (p->type == STR_AST) {
    } else if (p->type == EQUAL_AST) {
        v0->location = used_memory;
        v0->unit_size = 8;
        v0->negative = false;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
        dfs2(p->list[0]);
        dfs2(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,1);
        setValue(v0,v2,0);

        if (v0->type == UINT_TYPE) {
            equalUnsigned(v0);
        } else if (v0->type == INT_TYPE) {
            // todo
        } else if (v0->type == FIXED_TYPE) {
            // todo
        } else if (v0->type == CHAR_TYPE) {
            // todo
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
    } else if (p->type == NOTEQUAL_AST) {
    } else if (p->type == LESS_AST) {
        v0->location = used_memory;
        v0->unit_size = 8;
        v0->negative = false;
        val_list[list_size++] = v0;
        used_memory += size(v0)*v0->unit_size;
        dfs2(p->list[0]);
        dfs2(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,1);
        setValue(v0,v2,0);

        if (v0->type == UINT_TYPE) {
            lessUnsigned(v0);
        } else if (v0->type == INT_TYPE) {
            // todo
        } else if (v0->type == FIXED_TYPE) {
            // todo
        } else if (v0->type == CHAR_TYPE) {
            // todo
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
    } else if (p->type == LESSEQUAL_AST) {
    } else if (p->type == GREATER_AST) {
    } else if (p->type == GREATEREQUAL_AST) {
    } else if (p->type == IDENT_AST) {
        for (int i = 0;i < list_size;++i) if (strcmp(val_list[i]->ident,p->str) == 0) {
            *v0 = *val_list[i];
            break;
        }
    } else if (p->type == SCAN_AST) {
        dfs2(p->list[0]);
        Variable *v1 = p->list[0]->v;
        clear(v1,0,0,size(v1));
        if (v1->type == UINT_TYPE) {
            scanUint(v1);
        } else if (v1->type == INT_TYPE) {
            scanInt(v1);
        } else if (v1->type == FIXED_TYPE) {
            scanFixed(v1);
        } else if (v1->type == CHAR_TYPE) {
            scanChar(v1);
        } else if (v1->type == BOOL_TYPE) {
        } else {}
    } else if (p->type == PRINT_AST) {
        if (p->list[0]->type == STR_AST) {
            printStr(used_memory,p->list[0]->str);
        } else if (p->list[0]->type == IDENT_AST) {
            dfs2(p->list[0]);
            Variable *v1 = p->list[0]->v;
            if (v1->type == UINT_TYPE) {
                printUint(v1);
            } else if (v1->type == INT_TYPE) {
                printInt(v1);
            } else if (v1->type == FIXED_TYPE) {
                printFixed(v1);
            } else if (v1->type == CHAR_TYPE) {
                printChar(v1);
            } else if (v1->type == BOOL_TYPE) {
                printChar(v1);
            } else {}
        } else {}
    } else if (p->type == MAIN_AST) {
        for (int i = 0;i < p->n;++i) dfs2(p->list[i]);
        // for (int i = 0;i < list_size;++i) printf("%d\n",val_list[i]->location);
    } else if (p->type == STATEMENTS_AST) {
        for (int i = 0;i < p->n;++i) dfs2(p->list[i]);
    } else {
    }
}
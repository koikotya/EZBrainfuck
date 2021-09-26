#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "generator.h"

int list_size = 0;
Variable *val_list[256];

int used_memory = 0;

int min(int a,int b) {
    return (a < b ? a : b);
}

int size(Variable *v) {
    return v->idegit+v->fdegit+v->sign;
}

int getIndex(Operation op) {
    int res = 0;
    if (op == PLUS_TEMP) res = 1;
    else if (op == MINUS_TEMP) res = 1;
    else if (op == TIMES_TEMP) res = 4;
    else if (op == DIVIDE_TEMP) res = 7;
    else if (op == MOD_TEMP) res = 1;
    else ;
    return res;
}

void freeVariable(Variable *v) {
    list_size--;
    used_memory = val_list[list_size]->location;
    free(v);
}

void setLiteral(Variable *v1,Variable *v2,int index1) {
    if (v1->type == UINT) {
        setInteger(v1,index1,v2->ident);
    } else if (v1->type == INT) {
        setInteger(v1,index1,v2->ident);
        setSign(v1,index1,v2->negative);
    } else if (v1->type == FIXED) {
        setDecimal(v1,index1,v2->ident,v2->op);
        setSign(v1,index1,v2->negative);
    } else if (v1->type == CHAR) {
        setChar(v1,index1,v2->ident);
    } else if (v1->type == BOOL) {
        setChar(v1,index1,v2->ident);
    } else {
        printf("error setLiteral\n");
    }
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
        if ((v1->type == INT || v1->type == FIXED) && (v2->type == INT || v2->type == FIXED)) {
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
        if ((v1->type == INT || v1->type == FIXED) && (v2->type == INT || v2->type == FIXED)) {
            copy(v1,v2,size(v1)-1,size(v2)-1,index1,v2_index,1,1);
        }
        turnSign(v1,index1,v2->negative);
    }
};

// todo:激ヤバ実装なのでどうにかする
// todo:dfsの分割。1回目のdfsで桁数などを取得、2回目のdfsでメモリの位置を確定、コード生成
Variable *dfs(Node *p, Variable *t) {
    Variable *res = NULL;
    if (p == NULL) return res;
    if (p->type == PLUS_AST) {
        if (p->n == 1) {
            res = dfs(p->list[0],t);
        } else {
            res = (Variable *)malloc(sizeof(Variable));
            res->type = t->type;
            res->op = PLUS_TEMP;
            res->idegit = t->idegit;
            res->fdegit = t->fdegit;
            res->sign = t->sign;
            res->location = used_memory;
            res->unit_size = 8;
            res->negative = false;
            val_list[list_size++] = res;
            used_memory += size(res)*res->unit_size;
            
            Variable *v1 = dfs(p->list[0],t);
            Variable *v2 = dfs(p->list[1],t);
            setValue(res,v1,1);
            setValue(res,v2,0);

            movePointer(0,res->location);
            if (t->type == UINT) {
                add(res->idegit);
            } else if (t->type == INT) {
                signedAdd(res->idegit);
            } else if (t->type == FIXED) {
                signedAdd(res->idegit+res->fdegit);
            } else if (t->type == CHAR) {
                charAdd();
            } else if (t->type == BOOL) {
                // 定義しない
            } else {
                // error
            }
            movePointer(res->location,0);
        }
    } else if (p->type == MINUS_AST) {
        if (p->n == 1) {
            res = dfs(p->list[0],t);
            res->negative = (!res->negative);
        } else {
            res = (Variable *)malloc(sizeof(Variable));
            res->type = t->type;
            res->op = MINUS_TEMP;
            res->idegit = t->idegit;
            res->fdegit = t->fdegit;
            res->sign = t->sign;
            res->location = used_memory;
            res->unit_size = 8;
            res->negative = false;
            val_list[list_size++] = res;
            used_memory += size(res)*res->unit_size;
            
            Variable *v1 = dfs(p->list[0],t);
            Variable *v2 = dfs(p->list[1],t);
            setValue(res,v1,1);
            setValue(res,v2,0);

            movePointer(0,res->location);
            if (t->type == UINT) {
                sub(res->idegit);
            } else if (t->type == INT) {
                signedSub(res->idegit);
            } else if (t->type == FIXED) {
                signedSub(res->idegit+res->fdegit);
            } else if (t->type == CHAR) {
                charSub();
            } else if (t->type == BOOL) {
                // 定義しない
            } else {
                // error
            }
            movePointer(res->location,0);
        }
    } else if (p->type == TIMES_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = t->type;
        res->op = TIMES_TEMP;
        res->idegit = t->idegit+t->idegit+t->fdegit;
        res->fdegit = t->fdegit;
        res->sign = t->sign;
        res->location = used_memory;
        res->unit_size = 8;
        res->negative = false;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
        
        Variable *v1 = dfs(p->list[0],t);
        Variable *v2 = dfs(p->list[1],t);
        setValue(res,v1,1);
        setValue(res,v2,0);

        res->idegit = t->idegit+t->idegit;
        res->fdegit = t->fdegit+t->fdegit;

        movePointer(0,res->location);
        if (t->type == UINT) {
            multShort(t->idegit);
        } else if (t->type == INT) {
            signedMultShort(t->idegit);
        } else if (t->type == FIXED) {
            signedMultShort(t->idegit+t->fdegit);
        } else if (t->type == CHAR) {
            // 定義しない
        } else if (t->type == BOOL) {
            // 定義しない
        } else {
            // error
        }
        movePointer(res->location,0);
    } else if (p->type == DIVIDE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = t->type;
        res->op = DIVIDE_TEMP;
        res->idegit = t->idegit+t->idegit;
        res->fdegit = t->fdegit+t->fdegit;
        res->sign = t->sign;
        res->location = used_memory;
        res->unit_size = 8;
        res->negative = false;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
        
        Variable *v1 = dfs(p->list[0],t);
        Variable *v2 = dfs(p->list[1],t);
        setValue(res,v1,1);

        res->idegit = t->idegit+t->idegit+t->fdegit;
        res->fdegit = t->fdegit;
        setValue(res,v2,0);

        movePointer(0,res->location);
        if (t->type == UINT) {
            divide(t->idegit);
        } else if (t->type == INT) {
            signedDivide(t->idegit);
        } else if (t->type == FIXED) {
            signedDivide(t->idegit+t->fdegit);
        } else if (t->type == CHAR) {
            // 定義しない
        } else if (t->type == BOOL) {
            // 定義しない
        } else {
            // error
        }
        movePointer(res->location,0);
        clear(res,0,0,size(res));
        clear(res,0,1,size(res));
    } else if (p->type == MOD_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = t->type;
        res->op = MOD_TEMP;
        res->idegit = t->idegit+t->idegit;
        res->fdegit = t->fdegit+t->fdegit;
        res->sign = t->sign;
        res->location = used_memory;
        res->unit_size = 8;
        res->negative = false;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
        
        Variable *v1 = dfs(p->list[0],t);
        Variable *v2 = dfs(p->list[1],t);
        setValue(res,v1,1);

        res->idegit = t->idegit+t->idegit+t->fdegit;
        res->fdegit = t->fdegit;
        setValue(res,v2,0);

        movePointer(0,res->location);
        if (t->type == UINT) {
            divide(t->idegit);
        } else if (t->type == INT) {
            signedDivide(t->idegit);
        } else if (t->type == FIXED) {
            signedDivide(t->idegit+t->fdegit);
        } else if (t->type == CHAR) {
            // 定義しない
        } else if (t->type == BOOL) {
            // 定義しない
        } else {
            // error
        }
        movePointer(res->location,0);
        clear(res,0,0,size(res));
        clear(res,0,7,size(res));
    } else if (p->type == ASSIGN_AST) {
        Variable *v1 = dfs(p->list[0],NULL);
        Variable *v2 = dfs(p->list[1],v1);
        clear(v1,0,0,size(v1));
        setValue(v1,v2,0);
    } else if (p->type == INTNUMBER_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = INT_LITERAL;
        res->ident = p->str;
    } else if (p->type == DECIMALNUMBER_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = DECIMAL_LITERAL;
        res->ident = p->str;
    } else if (p->type == IF_AST) {
    } else if (p->type == IF_ELSE_AST) {
    } else if (p->type == WHILE_AST) {
    } else if (p->type == UINT_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = UINT;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = 0;
        res->sign = false;
        res->location = used_memory;
        res->unit_size = 3;
        res->negative = false;
        res->ident = p->list[1]->str;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
    } else if (p->type == INT_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = INT;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = 0;
        res->sign = true;
        res->location = used_memory;
        res->unit_size = 3;
        res->negative = false;
        res->ident = p->list[1]->str;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
    } else if (p->type == FIXED_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = FIXED;
        res->op = NOT_OP;
        res->idegit = atoi(p->list[0]->str);
        res->fdegit = atoi(p->list[1]->str);
        res->sign = true;
        res->location = used_memory;
        res->unit_size = 3;
        res->negative = false;
        res->ident = p->list[2]->str;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
    } else if (p->type == BOOL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = BOOL;
        res->op = NOT_OP;
        res->idegit = 1;
        res->fdegit = 0;
        res->sign = false;
        res->location = used_memory;
        res->unit_size = 2;
        res->negative = false;
        res->ident = p->list[0]->str;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
    } else if (p->type == CHAR_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->type = CHAR;
        res->op = NOT_OP;
        res->idegit = 1;
        res->fdegit = 0;
        res->sign = false;
        res->location = used_memory;
        res->unit_size = 2;
        res->negative = false;
        res->ident = p->list[0]->str;
        val_list[list_size++] = res;
        used_memory += size(res)*res->unit_size;
    } else if (p->type == STR_AST) {
    } else if (p->type == EQUAL_AST) {
    } else if (p->type == NOTEQUAL_AST) {
    } else if (p->type == LESS_AST) {
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
        Variable *v = dfs(p->list[0],NULL);
        clear(v,0,0,size(v));
        if (v->type == UINT) {
            scanUint(v);
        } else if (v->type == INT) {
            scanInt(v);
        } else if (v->type == FIXED) {
            scanFixed(v);
        } else if (v->type == CHAR) {
            scanChar(v);
        } else if (v->type == BOOL) {
        } else {}
    } else if (p->type == PRINT_AST) {
        if (p->list[0]->type == STR_AST) {
        } else if (p->list[0]->type == IDENT_AST) {
            Variable *v = dfs(p->list[0],NULL);
            if (v->type == UINT) {
                printUint(v);
            } else if (v->type == INT) {
                printInt(v);
            } else if (v->type == FIXED) {
                printFixed(v);
            } else if (v->type == CHAR) {
                printChar(v);
            } else if (v->type == BOOL) {
                printChar(v);
            } else {}
        } else {}
    } else if (p->type == MAIN_AST) {
        for (int i = 0;i < p->n;++i) dfs(p->list[i],NULL);
        // for (int i = 0;i < list_size;++i) printf("%d\n",val_list[i]->location);
    } else if (p->type == STATEMENTS_AST) {
        for (int i = 0;i < p->n;++i) dfs(p->list[i],NULL);
    } else {
    }
    return res;
}
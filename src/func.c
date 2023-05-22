#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "func.h"
#include "bf.h"

int size(Variable *v) {
    return v->idegit+v->fdegit+(v->sign ? 1 : 0);
}

int getIndex(Operation op) {
    int res = 0;
    if (op == PLUS_OP) res = 2;
    else if (op == MINUS_OP) res = 2;
    else if (op == TIMES_OP) res = 5;
    else if (op == DIVIDE_OP) res = 1;
    else if (op == MOD_OP) res = 5;
    else if (op == EQUAL_COND) res = 3;
    else if (op == NOTEQUAL_COND) res = 5;
    else if (op == LESS_COND) res = 1;
    else if (op == GREATEREQUAL_COND) res = 2;
    else if (op == NOT_OP) res = 1;
    else ;
    return res;
}

void allocate(Variable *v,int type) {
    int id = (used_memory[0] > used_memory[1] ? 0 : 1);
    if (type == 0) {
    } else if (type == 1) {
        id = (1^id);
    } else {
    }

    v->address = used_memory[id];
    val_list[list_size++] = v;
    used_memory[id] += size(v)*v->unit_size;
}

void freeVariable(Variable *v) {
    list_size--;
    if (val_list[list_size]->address%8 == 0) {
        used_memory[0] = val_list[list_size]->address;
    } else {
        used_memory[1] = val_list[list_size]->address;
    }
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
        printErr("error setLiteral\n");
    }
}

Type castType(Type t1,Type t2) {
    return (t1 > t2 ? t1 : t2);
}

// v2をv1のメモリに書き込む
// v1のどのindexに書き込みかは引数で指定
void setValue(Variable *v1,Variable *v2,int index1) {
    int v2_index = 1;
    if (v2->op == INT_LITERAL || v2->op == DECIMAL_LITERAL) {
        // v2がリテラルの場合
        setLiteral(v1,v2,index1);
    } else if (v2->op == PLUS_OP || v2->op == MINUS_OP || v2->op == TIMES_OP || v2->op == DIVIDE_OP || v2->op == MOD_OP) {
        // v2が計算中の値の場合
        // v2をv1にムーブ
        v2_index = getIndex(v2->op);
        int fdegit = min(v1->fdegit,v2->fdegit);
        int idegit = min(v1->idegit,v2->idegit);
        if (v1->type == CHAR_TYPE && CHAR_TYPE < v2->type) {
            Variable *v3;
            v3 = (Variable *)malloc(sizeof(Variable));
            allocate(v3,0);
            moveIntToChar(v3,v2,1,v2_index);
            move(v1,v3,0,0,index1,1,1);
            freeVariable(v3);
        } else if (v2->type == CHAR_TYPE && CHAR_TYPE < v1->type) {
            Variable *v3;
            v3 = (Variable *)malloc(sizeof(Variable));
            v3->idegit = 3;
            v3->unit_size = 8;
            allocate(v3,0);
            moveCharToInt(v3,v2,1,v2_index);
            move(v1,v3,v1->fdegit,0,index1,2,min(v1->idegit,v3->idegit));
            clear(v3,0,1,3);
            freeVariable(v3);
        } else {
            move(v1,v2,v1->fdegit-fdegit,v2->fdegit-fdegit,index1,v2_index,fdegit+idegit);
            // 符号の処理
            if ((v1->type == INT_TYPE || v1->type == FIXED_TYPE) && (v2->type == INT_TYPE || v2->type == FIXED_TYPE)) {
                move(v1,v2,size(v1)-1,size(v2)-1,index1,v2_index,1);
            }
            turnSign(v1,index1,v2->negative);
        }
        clear(v2,0,v2_index,size(v2));
        freeVariable(v2);
    } else {
        // v2が変数の場合
        // v2をv1にコピー
        int fdegit = min(v1->fdegit,v2->fdegit);
        int idegit = min(v1->idegit,v2->idegit);
        if (v1->type == CHAR_TYPE && CHAR_TYPE < v2->type) {
            Variable *v3;
            v3 = (Variable *)malloc(sizeof(Variable));
            allocate(v3,0);
            copyIntToChar(v3,v2,1,v2_index,2);
            move(v1,v3,0,0,index1,1,1);
            freeVariable(v3);
        } else if (v2->type == CHAR_TYPE && CHAR_TYPE < v1->type) {
            Variable *v3;
            v3 = (Variable *)malloc(sizeof(Variable));
            v3->idegit = 3;
            v3->unit_size = 8;
            allocate(v3,0);
            copyCharToInt(v3,v2,1,v2_index,2);
            move(v1,v3,v1->fdegit,0,index1,2,min(v1->idegit,v3->idegit));
            clear(v3,0,1,3);
            freeVariable(v3);
        } else {
            copy(v1,v2,v1->fdegit-fdegit,v2->fdegit-fdegit,index1,v2_index,fdegit+idegit,2);
            // 符号の処理
            if ((v1->type == INT_TYPE || v1->type == FIXED_TYPE) && (v2->type == INT_TYPE || v2->type == FIXED_TYPE)) {
                copy(v1,v2,size(v1)-1,size(v2)-1,index1,v2_index,1,2);
            }
            turnSign(v1,index1,v2->negative);
        }
    }
};

#ifdef __EMSCRIPTEN__
extern char err[1000];

void printErr(char *s) {
    strcat(err, s);
}

extern char result[1<<20];

void output(char *s) {
    strcat(result, s);
}

#else
void printErr(char *s) {
    fprintf(stderr,"%s",s);
}

extern FILE *Fresult;

void output(char *s) {
    fprintf(Fresult,s);
}
#endif
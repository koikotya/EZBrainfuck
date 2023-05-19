#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
// #include "generator.h"

int list_size = 0;
Variable *val_list[100010];

int used_memory[2] = {0,4};

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) < (b)) ? (b) : (a))

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

void setMemorySize(Node *p) {
    Variable *res = NULL;
    if (p == NULL) return;
    if (p->type == PLUS_AST) {
        if (p->n == 1) {
            setMemorySize(p->list[0]);
            res = (Variable *)malloc(sizeof(Variable));
            *res = *p->list[0]->v;
            res->sign = true;
            if (res->type == UINT_TYPE) res->type = INT_TYPE;
        } else {
            res = (Variable *)malloc(sizeof(Variable));
            res->op = PLUS_OP;
            res->unit_size = 8;
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
        }
    } else if (p->type == MINUS_AST) {
        if (p->n == 1) {
            setMemorySize(p->list[0]);
            res = (Variable *)malloc(sizeof(Variable));
            *res = *p->list[0]->v;
            res->sign = true;
            if (res->type == UINT_TYPE) res->type = INT_TYPE;
            res->negative = (!res->negative);
        } else {
            res = (Variable *)malloc(sizeof(Variable));
            res->op = MINUS_OP;
            res->unit_size = 8;
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
        }
    } else if (p->type == TIMES_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = TIMES_OP;
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == IF_ELSE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->unit_size = 8;
        setMemorySize(p->list[0]);
        setMemorySize(p->list[1]);
        setMemorySize(p->list[2]);
        res->idegit = 0;
        res->fdegit = 0;
        res->sign = true;
    } else if (p->type == WHILE_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
        res->negative = false;
        res->ident = p->list[0]->str;
        val_list[list_size++] = res;
    } else if (p->type == STR_AST) {
    } else if (p->type == EQUAL_AST) {
        res = (Variable *)malloc(sizeof(Variable));
        res->op = EQUAL_COND;
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
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
        res->unit_size = 8;
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

void generate(Node *p) {
    if (p == NULL) return;
    Variable *v0 = p->v;
    if (p->type == PLUS_AST) {
        if (p->n == 1) {
            generate(p->list[0]);
            Variable *v1 = p->list[0]->v;
            v0->address = v1->address;
        } else {
            v0->unit_size = 8;
            v0->negative = false;
            allocate(v0,0);
            generate(p->list[0]);
            generate(p->list[1]);
            Variable *v1 = p->list[0]->v;
            Variable *v2 = p->list[1]->v;
            setValue(v0,v1,2);
            setValue(v0,v2,1);

            movePointer(0,v0->address);
            if (v0->type == UINT_TYPE) {
                add(v0->idegit);
            } else if (v0->type == INT_TYPE) {
                addSigned(v0->idegit);
            } else if (v0->type == FIXED_TYPE) {
                addSigned(v0->idegit+v0->fdegit);
            } else if (v0->type == CHAR_TYPE) {
                addChar();
            } else if (v0->type == BOOL_TYPE) {
                // 定義しない
            } else {
                // error
            }
            movePointer(v0->address,0);
        }
    } else if (p->type == MINUS_AST) {
        if (p->n == 1) {
            generate(p->list[0]);
            Variable *v1 = p->list[0]->v;
            v0->address = v1->address;
        } else {
            v0->unit_size = 8;
            v0->negative = false;
            allocate(v0,0);
            generate(p->list[0]);
            generate(p->list[1]);
            Variable *v1 = p->list[0]->v;
            Variable *v2 = p->list[1]->v;
            setValue(v0,v1,2);
            setValue(v0,v2,1);

            movePointer(0,v0->address);
            if (v0->type == UINT_TYPE) {
                sub(v0->idegit);
            } else if (v0->type == INT_TYPE) {
                subSigned(v0->idegit);
            } else if (v0->type == FIXED_TYPE) {
                subSigned(v0->idegit+v0->fdegit);
            } else if (v0->type == CHAR_TYPE) {
                subChar();
            } else if (v0->type == BOOL_TYPE) {
                // 定義しない
            } else {
                // error
            }
            movePointer(v0->address,0);
        }
    } else if (p->type == TIMES_AST) {
        v0->idegit = v0->idegit+v0->idegit+v0->fdegit;

        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,2);
        setValue(v0,v2,1);

        v0->idegit -= v0->fdegit;
        v0->fdegit += v0->fdegit;

        movePointer(0,v0->address);
        if (v0->type == UINT_TYPE) {
            multShort(v0->idegit/2);
        } else if (v0->type == INT_TYPE) {
            multSignedShort(v0->idegit/2);
        } else if (v0->type == FIXED_TYPE) {
            multSignedShort((v0->idegit+v0->fdegit)/2);
        } else if (v0->type == CHAR_TYPE) {
            // 定義しない
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
        movePointer(v0->address,0);
    } else if (p->type == DIVIDE_AST) {
        v0->idegit = v0->idegit+v0->idegit;
        v0->fdegit = v0->fdegit+v0->fdegit;

        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,5);

        v0->fdegit /= 2;
        v0->idegit += v0->fdegit;
        setValue(v0,v2,2);

        movePointer(0,v0->address);
        if (v0->type == UINT_TYPE) {
            divide(v0->idegit/2);
        } else if (v0->type == INT_TYPE) {
            divideSigned(v0->idegit/2);
        } else if (v0->type == FIXED_TYPE) {
            divideSigned((v0->idegit+v0->fdegit)/2);
        } else if (v0->type == CHAR_TYPE) {
            // 定義しない
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
        movePointer(v0->address,0);
        clear(v0,0,getIndex(MOD_OP),size(v0));
    } else if (p->type == MOD_AST) {
        v0->idegit = v0->idegit+v0->idegit+v0->fdegit;
        v0->fdegit = v0->fdegit;

        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,5);

        setValue(v0,v2,2);

        movePointer(0,v0->address);
        if (v0->type == UINT_TYPE) {
            divide(v0->idegit/2);
        } else if (v0->type == INT_TYPE) {
            divideSigned(v0->idegit/2);
        } else if (v0->type == FIXED_TYPE) {
            divideSigned((v0->idegit+v0->fdegit)/2);
        } else if (v0->type == CHAR_TYPE) {
            // 定義しない
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
        movePointer(v0->address,0);
        clear(v0,0,getIndex(DIVIDE_OP),size(v0));
    } else if (p->type == ASSIGN_AST) {
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        if (v1->ident != NULL && v2->ident != NULL && strcmp(v1->ident,v2->ident) == 0) {
            turnSign(v1,1,v2->negative);
        } else {
            clear(v1,0,1,size(v1));
            setValue(v1,v2,1);
        }
    } else if (p->type == INTNUMBER_AST) {
    } else if (p->type == DECIMALNUMBER_AST) {
    } else if (p->type == IF_AST) {
        allocate(v0,1);

        generate(p->list[0]);
        Variable *v1 = p->list[0]->v;
        move(v0,v1,0,size(v1)-1,1,getIndex(v1->op),1);
        freeVariable(v1);
        ifBegin(v0);

        generate(p->list[1]);

        ifEnd(v0);
        freeVariable(v0);
    } else if (p->type == IF_ELSE_AST) {
        allocate(v0,1);

        generate(p->list[0]);
        Variable *v1 = p->list[0]->v;
        move(v0,v1,0,size(v1)-1,1,getIndex(v1->op),1);
        freeVariable(v1);
        ifElseBegin(v0);

        generate(p->list[1]);

        ifElseMid(v0);

        generate(p->list[2]);

        ifElseEnd(v0);
        freeVariable(v0);
    } else if (p->type == WHILE_AST) {
        allocate(v0,1);

        whileBegin(v0);
        generate(p->list[0]);
        Variable *v1 = p->list[0]->v;
        copy(v0,v1,0,size(v1)-1,1,getIndex(v1->op),1,2);
        move(v0,v1,0,size(v1)-1,2,getIndex(v1->op),1);
        whileMid(v0);
        generate(p->list[1]);
        whileEnd(v0);

        freeVariable(v1);
        freeVariable(v0);
    } else if (p->type == UINT_AST) {
        allocate(v0,1);
    } else if (p->type == INT_AST) {
        allocate(v0,1);
    } else if (p->type == FIXED_AST) {
        allocate(v0,1);
    } else if (p->type == BOOL_AST) {
        allocate(v0,1);
    } else if (p->type == CHAR_AST) {
        allocate(v0,1);
    } else if (p->type == STR_AST) {
    } else if (p->type == EQUAL_AST) {
        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,2);
        setValue(v0,v2,1);

        if (v0->type == UINT_TYPE) {
            equalUnsigned(v0);
        } else if (v0->type == INT_TYPE) {
            equalSigned(v0);
        } else if (v0->type == FIXED_TYPE) {
            equalSigned(v0);
        } else if (v0->type == CHAR_TYPE) {
            equalChar(v0);
        } else if (v0->type == BOOL_TYPE) {
            equalChar(v0);
        } else {
            // error
        }
    } else if (p->type == NOTEQUAL_AST) {
        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,3);
        setValue(v0,v2,2);

        if (v0->type == UINT_TYPE) {
            notEqualUnsigned(v0);
        } else if (v0->type == INT_TYPE) {
            notEqualSigned(v0);
        } else if (v0->type == FIXED_TYPE) {
            notEqualSigned(v0);
        } else if (v0->type == CHAR_TYPE) {
            notEqualChar(v0);
        } else if (v0->type == BOOL_TYPE) {
            notEqualChar(v0);
        } else {
            // error
        }
    } else if (p->type == LESS_AST) {
        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,2);
        setValue(v0,v2,1);

        if (v0->type == UINT_TYPE) {
            lessUnsigned(v0);
        } else if (v0->type == INT_TYPE) {
            lessSigned(v0);
        } else if (v0->type == FIXED_TYPE) {
            lessSigned(v0);
        } else if (v0->type == CHAR_TYPE) {
            lessChar(v0);
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
    } else if (p->type == GREATEREQUAL_AST) {
        v0->unit_size = 8;
        v0->negative = false;
        allocate(v0,0);
        generate(p->list[0]);
        generate(p->list[1]);
        Variable *v1 = p->list[0]->v;
        Variable *v2 = p->list[1]->v;
        setValue(v0,v1,2);
        setValue(v0,v2,1);

        if (v0->type == UINT_TYPE) {
            greaterEqualUnsigned(v0);
        } else if (v0->type == INT_TYPE) {
            greaterEqualSigned(v0);
        } else if (v0->type == FIXED_TYPE) {
            greaterEqualSigned(v0);
        } else if (v0->type == CHAR_TYPE) {
            greaterEqualChar(v0);
        } else if (v0->type == BOOL_TYPE) {
            // 定義しない
        } else {
            // error
        }
    } else if (p->type == IDENT_AST) {
        for (int i = 0;i < list_size;++i) if (val_list[i]->ident != NULL && strcmp(val_list[i]->ident,p->str) == 0) {
            *v0 = *val_list[i];
            break;
        }
    } else if (p->type == SCAN_AST) {
        generate(p->list[0]);
        Variable *v1 = p->list[0]->v;
        clear(v1,0,1,size(v1));
        if (v1->type == UINT_TYPE) {
            scanUint(v1,1);
        } else if (v1->type == INT_TYPE) {
            scanInt(v1,1);
        } else if (v1->type == FIXED_TYPE) {
            scanFixed(v1,1);
        } else if (v1->type == CHAR_TYPE) {
            scanChar(v1,1);
        } else if (v1->type == BOOL_TYPE) {
        } else {}
    } else if (p->type == PRINT_AST) {
        if (p->list[0]->type == STR_AST) {
            printStr(0,p->list[0]->str);
        } else if (p->list[0]->type == IDENT_AST) {
            generate(p->list[0]);
            Variable *v1 = p->list[0]->v;
            if (v1->type == UINT_TYPE) {
                printUint(v1,1);
            } else if (v1->type == INT_TYPE) {
                printInt(v1,1);
            } else if (v1->type == FIXED_TYPE) {
                printFixed(v1,1);
            } else if (v1->type == CHAR_TYPE) {
                printChar(v1,1);
            } else if (v1->type == BOOL_TYPE) {
                printChar(v1,1);
            } else {}
        } else {}
    } else if (p->type == MAIN_AST) {
        for (int i = 0;i < p->n;++i) generate(p->list[i]);
        // for (int i = 0;i < list_size;++i) printf("%d\n",val_list[i]->address);
    } else if (p->type == STATEMENTS_AST) {
        for (int i = 0;i < p->n;++i) generate(p->list[i]);
    } else {
    }
}
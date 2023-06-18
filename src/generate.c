#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "func.h"
#include "bf.h"

extern int list_size;
extern Variable *val_list[100010];

int used_memory[2] = {0,4};

void generate(Node *p) {
    if (p == NULL) return;
    Variable *v0 = p->v;
    if (p->type == UNARY_PLUS_AST) {
        generate(p->childs[0]);
        Variable *v1 = p->childs[0]->v;
        v0->address = v1->address;
    } else if (p->type == UNARY_MINUS_AST) {
        generate(p->childs[0]);
        Variable *v1 = p->childs[0]->v;
        v0->address = v1->address;
    } else if (p->type == PLUS_AST) {
        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
    } else if (p->type == MINUS_AST) {
        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
    } else if (p->type == TIMES_AST) {
        v0->idegit = v0->idegit+v0->idegit+v0->fdegit;

        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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

        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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

        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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

        generate(p->childs[0]);
        Variable *v1 = p->childs[0]->v;
        move(v0,v1,0,size(v1)-1,1,getIndex(v1->op),1);
        freeVariable(v1);
        ifBegin(v0);

        generate(p->childs[1]);

        ifEnd(v0);
        freeVariable(v0);
    } else if (p->type == IF_ELSE_AST) {
        allocate(v0,1);

        generate(p->childs[0]);
        Variable *v1 = p->childs[0]->v;
        move(v0,v1,0,size(v1)-1,1,getIndex(v1->op),1);
        freeVariable(v1);
        ifElseBegin(v0);

        generate(p->childs[1]);

        ifElseMid(v0);

        generate(p->childs[2]);

        ifElseEnd(v0);
        freeVariable(v0);
    } else if (p->type == WHILE_AST) {
        allocate(v0,1);

        whileBegin(v0);
        generate(p->childs[0]);
        Variable *v1 = p->childs[0]->v;
        copy(v0,v1,0,size(v1)-1,1,getIndex(v1->op),1,2);
        move(v0,v1,0,size(v1)-1,2,getIndex(v1->op),1);
        whileMid(v0);
        generate(p->childs[1]);
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
        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
        allocate(v0,0);
        generate(p->childs[0]);
        generate(p->childs[1]);
        Variable *v1 = p->childs[0]->v;
        Variable *v2 = p->childs[1]->v;
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
        generate(p->childs[0]);
        Variable *v1 = p->childs[0]->v;
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
        if (p->childs[0]->type == STR_AST) {
            printStr(0,p->childs[0]->str);
        } else if (p->childs[0]->v->op == INT_LITERAL || p->childs[0]->v->op == DECIMAL_LITERAL) {
            Variable *v1 = p->childs[0]->v;
            if (v1->op == INT_LITERAL && v1->negative) {
                v1->sign = true;
                v1->type = INT_TYPE;
            }
            int index = 1;
            allocate(v1,index);
            setValue(v1,v1,index);
            if (v1->type == UINT_TYPE) {
                printUint(v1,index);
            } else if (v1->type == INT_TYPE) {
                printInt(v1,index);
            } else if (v1->type == FIXED_TYPE) {
                printFixed(v1,index);
            } else {}
            clear(v1,0,index,size(v1));
            freeVariable(v1);
        } else {
            generate(p->childs[0]);
            Variable *v1 = p->childs[0]->v;
            int index = getIndex(v1->op);
            turnSign(v1,index,v1->negative);
            if (v1->type == UINT_TYPE) {
                printUint(v1,index);
            } else if (v1->type == INT_TYPE) {
                printInt(v1,index);
            } else if (v1->type == FIXED_TYPE) {
                printFixed(v1,index);
            } else if (v1->type == CHAR_TYPE) {
                printChar(v1,index);
            } else if (v1->type == BOOL_TYPE) {
                printChar(v1,index);
            } else {}
            if (v1->op == NOT_OP) turnSign(v1,index,v1->negative);
            else clear(v1,0,index,size(v1));
        }
    } else if (p->type == MAIN_AST) {
        for (int i = 0;i < p->n;++i) generate(p->childs[i]);
        // for (int i = 0;i < list_size;++i) printf("%d\n",val_list[i]->address);
    } else if (p->type == STATEMENTS_AST) {
        for (int i = 0;i < p->n;++i) generate(p->childs[i]);
    } else {
    }
}
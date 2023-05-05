#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
// #include "generator.h"

#ifdef __EMSCRIPTEN__
extern char result[1<<20];

void output(char *s) {
    strcat(result, s);
}

#else
extern FILE *Fresult;

void output(char *s) {
    fprintf(Fresult,s);
}
#endif

void movePointer(int p1,int p2) {
    if (p1 < p2) {
        for (int i = 0;i < p2-p1;++i) output(">");
    } else {
        for (int i = 0;i < p1-p2;++i) output("<");
    }
}

void setSign(Variable* v,int index,bool sign) {
    if (sign == false) return;
    output("\nset sign\n");
    movePointer(0,v->location+v->unit_size*(v->fdegit+v->idegit)+index);
    output("+");
    movePointer(v->location+v->unit_size*(v->fdegit+v->idegit)+index,0);
    output("\nend set sign\n");
}

void turnSign(Variable* v,int index,bool sign) {
    if (sign == false) return;
    output("\nturn sign\n");
    movePointer(0,v->location+v->unit_size*(v->fdegit+v->idegit)+index);
    output(">+<[>-<-]>[<+>-]<");
    movePointer(v->location+v->unit_size*(v->fdegit+v->idegit)+index,0);
    output("\nend turn sign\n");
}

void setChar(Variable* v,int index,char *literal) {
    output("\nset char\n");
    int num = atoi(literal);
    movePointer(0,v->location+index);
    for (int i = 0;i < num;++i) output("+");
    movePointer(v->location+index,0);
    output("\nend set char\n");
}

void setInteger(Variable* v,int index,char *literal) {
    output("\nset integer\n");
    int len = strlen(literal);
    movePointer(0,v->location+index);
    for (int i = len-1;i >= 0;--i) {
        for (int j = 0;j < literal[i]-'0';++j) output("+");
        movePointer(0,v->unit_size);
    }
    movePointer(v->location+index+v->unit_size*len,0);
    output("\nend set integer\n");
}

void setDecimal(Variable* v,int index,char *literal,Operation op) {
    output("\nset decimal\n");
    if (op == INT_LITERAL) {
        int len = strlen(literal);
        movePointer(0,v->location+v->unit_size*v->fdegit+index);
        for (int i = len-1;i >= 0;--i) {
            for (int j = 0;j < literal[i]-'0';++j) output("+");
            movePointer(0,v->unit_size);
        }
        movePointer(v->location+v->unit_size*(v->fdegit+len)+index,0);
    } else if (op == DECIMAL_LITERAL) {
        char str1[256],str2[256];
        sscanf(literal,"%[0-9].%[0-9]",str1,str2);
        int len1 = strlen(str1),len2 = strlen(str2);
        movePointer(0,v->location+v->unit_size*v->fdegit+index);
        for (int i = len1-1;i >= 0;--i) {
            for (int j = 0;j < str1[i]-'0';++j) output("+");
            movePointer(0,v->unit_size);
        }
        movePointer(v->unit_size*len1,0);
        for (int i = 0;i < len2;++i) {
            movePointer(v->unit_size,0);
            for (int j = 0;j < str2[i]-'0';++j) output("+");
        }
        movePointer(0,v->unit_size*len2);
        movePointer(v->location+v->unit_size*v->fdegit+index,0);
    } else {
        print_err("error setDecimal\n");
    }
    output("\nend set decimal\n");
}

// todo:add関数その他にVriable構造体を渡すように改造
void add(int n) {
    output("\nadd\n");
    for (int i = 0;i < n;++i) output(">[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("\nend add\n");
}

void sub(int n) {
    output("\nsub\n");
    for (int i = 0;i < n;++i) output(">[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("\nend sub\n");
}

void addSigned(int n) {
    output("\nsigned add\n");
    // movePointer(0,n*8);
    // output("[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[+[-]<<<");
    // movePointer(n*8,0);
    // sub(n);
    // movePointer(0,n*8);
    // output("[->[>-<-]>+[<+>-]<<");

    // for (int i = 0;i < n;++i) output("<<<<<<+++++++++<[>-<-]<");
    // output(">+");
    // for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>");
    
    // output("<]>>>>-<]>[-<<<<");
    // movePointer(n*8,0);
    // add(n);
    // movePointer(0,n*8);
    // output(">>>>]<<<<");
    // movePointer(n*8,0);
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output(">[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>>");
    output(" [>[>-<-]>+[<+>-]<");
    for (int i = 0;i < n;++i) output("<<<<<<<+++++++++<[>-<-]");
    output("+");
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>");
    output("<[-]]>>>>-<+[-]]>[<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    output(">>>>-]<<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("\nend signed add\n");
}

void subSigned(int n) {
    output("\nigned sub\n");
    // movePointer(0,n*8);
    // output("[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[+[-]<<<");
    // movePointer(n*8,0);
    // add(n);
    // movePointer(0,n*8);
    // output(">>>>-<]>[-<<<<");
    // movePointer(n*8,0);
    // sub(n);
    // movePointer(0,n*8);

    // output("[->[>-<-]>+[<+>-]<<");
    // for (int i = 0;i < n;++i) output("<<<<<<+++++++++<[>-<-]<");
    // output(">+");
    // for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>");
    // output("<]>>>>]<<<<");

    // // output("[->[>-<-]>+[<+>-]<<]>>>>]<<<<");

    // movePointer(n*8,0);
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output(">[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    output(">>>>-<+[-]]>[<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>>");
    output("[>[>-<-]>+[<+>-]<");
    for (int i = 0;i < n;++i) output("<<<<<<<+++++++++<[>-<-]");
    output("+");
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>");
    output("<[-]]>>>>-]<<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("\nend signed sub\n");
}

void addChar() {
    output("\nchar add\n");
    output(">[>+<-]<");
    output("\nend char add\n");
}

void subChar() {
    output("\nchar sub\n");
    output(">[>-<-]<");
    output("\nend char sub\n");
}

void mult(int n) {
    output("\nmult\n");
    for (int i = 0;i < n;++i) {
        output(">[-");
        for (int i = 0;i < n;++i) output(">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>");
        for (int i = 0;i < n;++i) output("<<<<<<<<");
        output("]>");
        for (int i = 0;i < n;++i) output(">>>>>>>>");
        for (int i = 0;i < n;++i) output("<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
        output(">>>>>>");
    }
    for (int i = 0;i < n;++i) output(">>[-]>>>>>>");
    output("<<<<[>+<-]>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<<<<<<<<<");
    output("\nend mult\n");
}

void multSigned(int n) {
    output("\nsigned mult\n");
    for (int i = 0;i < n;++i) {
        output(">[-");
        for (int i = 0;i < n;++i) output(">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>");
        for (int i = 0;i < n;++i) output("<<<<<<<<");
        output("]>");
        for (int i = 0;i < n;++i) output(">>>>>>>>");
        for (int i = 0;i < n;++i) output("<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
        output(">>>>>>");
    }
    for (int i = 0;i < n;++i) output(">>[-]>>>>>>");
    output("<<<<[>+<-]>>>>>[>-<-]>[+[-]>>>+<<<]<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<<<<<<<<<");
    output("\nend signed mult\n");
}

void multShort(int n) {
    output("\nmult short\n");
    for (int i = 0;i < n;++i) output("+>>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("[->[-");
    for (int i = 0;i < n;++i) output(">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("]>");
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
    output(">>>>>>]");
    for (int i = 0;i < n;++i) output(">>[-]>>>>>>");
    output("<<<<[>+<-]>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<<<<<<<<<");
    output("\nend mult short\n");
}

void multSignedShort(int n) {
    output("\nsigned mult short\n");
    for (int i = 0;i < n;++i) output("+>>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("[->[-");
    for (int i = 0;i < n;++i) output(">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("]>");
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
    output(">>>>>>]");
    for (int i = 0;i < n;++i) output(">>[-]>>>>>>");
    output("<<<<[>+<-]>>>>>[>-<-]>[+[-]>>>+<<<]<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<<<<<<<<<");
    output("\nend signed mult short\n");
}

void divide(int n) {
    /*
    a/b = q..r
    0:
    1:q
    2:b
    3:
    4:
    5:a,r
    6:
    7:
    */
    output("\ndivide\n");
    // ｂを右にシフト
    for (int i = 0;i < n;++i) output(">>>>>>>>+");
    output("[-<<<<<<[");
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("+");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("-]<<]");
    // ループ用フラグを立てる
    for (int i = 0;i < n;++i) output(">>>>>>>>+");
    // 除算
    output("[");
    for (int i = 0;i < n;++i) output(">>[<<<<<<<<+>>>>>>>>-]>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("[<<<<<<<+>");
    for (int i = 0;i < n;++i) output("[>+>+<<-]>[<+>-]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output(">>");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+<<++++++++++>>>>>>>+<<<<<]<<-<]>>>>>>>>");
    output("[-");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("<<<->");
    for (int i = 0;i < n;++i) output("[>+>+<<-]>[<+>-]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output(">>>>>>-<<<<");
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    output("-]");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output(">>>>]<<<<<<<<]");
    for (int i = 0;i < n;++i) output(">>[-]>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("\nend divide\n");
}

void divideSigned(int n) {
    output("\nsigned divide\n");
    // ｂを右にシフト
    for (int i = 0;i < n;++i) output(">>>>>>>>+");
    output("[-<<<<<<[");
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("+");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("-]<<]");
    // ループ用フラグを立てる
    for (int i = 0;i < n;++i) output(">>>>>>>>+");
    // 除算
    output("[");
    for (int i = 0;i < n;++i) output(">>[<<<<<<<<+>>>>>>>>-]>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("[<<<<<<<+>");
    for (int i = 0;i < n;++i) output("[>+>+<<-]>[<+>-]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output(">>");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+<<++++++++++>>>>>>>+<<<<<]<<-<]>>>>>>>>");
    output("[-");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("<<<->");
    for (int i = 0;i < n;++i) output("[>+>+<<-]>[<+>-]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output(">>>>>>-<<<<");
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    output("-]");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output(">>>>]<<<<<<<<]");
    for (int i = 0;i < n;++i) output(">>>>>>>>>>>>>>>>");
    output(">>>>>[<+<<->>>-]<[>+<-]<<[+[-]<+>]<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<[-]<<");
    output("\nend signed divide\n");
}

// Unidenさんありがとうございます
void UnidenDivide(int n) {
    output("\ndivide Uniden\n");
    for (int i = 0;i < n;++i) output(">>>>>>>+>");
    output("<[<<<<<<<[>>>>>>>[-]>>>>>>>>[>>>>>>>>]");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("[->>>>>>>>]+[->>>>>>>>[>>>>>>>>]<<<<<<<+<[<<<<<<<<]>>>>>>>>]<<<<<<<-[");
    for (int i = 0;i < n;++i) output("<<<<<<<<([->>>>>>>>+<<<<<<<<]");
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output(">>+<<-]>>+[<+[<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+>>>>-<<<<<<<<<<<++++++++++>>]>+<<<-<]>>>>[-<<<<+>>>>]>>>>");
    output("[->-<]>>>+>[+]<<<<>]<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[-> ---------[>>+<]>[<]>-[+<<---------->> >>>>>+>>>>-<<<<<<<<<]>+< << ++++++++++<] >>>>[-<<<<+>>>>]>>>>");
    output("->>> >[+]<-[->>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("+");
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("<<<<]<->>+<<[>]>[<<<<");
    for (int i = 0;i < n;++i) output("<[->>>>>>>>+<<<<<<<<]<<<<<<<");
    for (int i = 0;i < n;++i) output(">>>>>>>>[-<<<<<<<<+>>>>>>>>]");
    output(">>>]>-<<]]<]<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("\nend divide Uniden\n");
}

// v2をv1にコピー
// 0番地からスタート
void move(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len) {
    output("\nmove\n");
    movePointer(0,v2->location);
    movePointer(0,start_unit2*v2->unit_size+index2);
    for (int i = 0;i < len;++i) {
        output("[");
        movePointer(v2->location+v2->unit_size*(start_unit2+i)+index2,v1->location+v1->unit_size*(start_unit1+i)+index1);
        output("+");
        movePointer(v1->location+v1->unit_size*(start_unit1+i)+index1,v2->location+v2->unit_size*(start_unit2+i)+index2);
        output("-]");
        movePointer(0,v2->unit_size);
    }
    movePointer(v2->unit_size*(start_unit2+len)+index2,0);
    movePointer(v2->location,0);
    output("\nend move\n");
}

void moveCharToInt(Variable *v1,Variable *v2,int index1,int index2) {
    output("\nmove char to int\n");
    movePointer(0,v2->location);
    movePointer(0,index2);

    output("[");
    movePointer(v2->location+index2,v1->location+index1);
    output("+");
    movePointer(v1->location+index1,v2->location+index2);
    output("-]");

    int n = v1->idegit;
    movePointer(v2->location+index2,v1->location+index1);
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");

    movePointer(index1,0);
    movePointer(v1->location,0);
    output("\nend move char to int\n");
}

void moveIntToChar(Variable *v1,Variable *v2,int index1,int index2) {
    output("\nmove int to char\n");
    movePointer(0,v2->location);
    movePointer(0,v2->unit_size*(v2->fdegit+v2->idegit));
    movePointer(0,index2);
    for (int i = v2->idegit-1;i >= 0;--i)  {
        movePointer(v2->unit_size,0);
        movePointer(v2->location+v2->unit_size*(v2->fdegit+i)+index2,v1->location+index1);
        output("[>+<-]>[<++++++++++>-]<");
        movePointer(v1->location+index1,v2->location+v2->unit_size*(v2->fdegit+i)+index2);
        output("[");
        movePointer(v2->location+v2->unit_size*(v2->fdegit+i)+index2,v1->location+index1);
        output("+");
        movePointer(v1->location+index1,v2->location+v2->unit_size*(v2->fdegit+i)+index2);
        output("-]");
    }
    movePointer(v2->unit_size*(v2->fdegit),0);
    movePointer(index2,0);
    movePointer(v2->location,0);
    output("\nend move int to char\n");
}

void copy(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len,int empty_index) {
    output("\ncopy\n");
    movePointer(0,v2->location);
    movePointer(0,start_unit2*v2->unit_size+index2);
    for (int i = 0;i < len;++i) {
        output("[");

        movePointer(index2,empty_index);
        output("+");
        movePointer(empty_index,index2);

        movePointer(v2->location+v2->unit_size*(start_unit2+i)+index2,v1->location+v1->unit_size*(start_unit1+i)+index1);
        output("+");
        movePointer(v1->location+v1->unit_size*(start_unit1+i)+index1,v2->location+v2->unit_size*(start_unit2+i)+index2);
        output("-]");

        movePointer(index2,empty_index);
        output("[");
        movePointer(empty_index,index2);
        output("+");
        movePointer(index2,empty_index);
        output("-]");
        movePointer(empty_index,index2);

        movePointer(0,v2->unit_size);
    }
    movePointer(v2->unit_size*(start_unit2+len)+index2,0);
    movePointer(v2->location,0);
    output("\nend copy\n");
}

void copyCharToInt(Variable *v1,Variable *v2,int index1,int index2,int empty_index) {
    output("\nmove copy to int\n");
    movePointer(0,v2->location);
    movePointer(0,index2);

    output("[");

    movePointer(index2,empty_index);
    output("+");
    movePointer(empty_index,index2);

    movePointer(v2->location+index2,v1->location+index1);
    output("+");
    movePointer(v1->location+index1,v2->location+index2);
    output("-]");

    movePointer(index2,empty_index);
    output("[");
    movePointer(empty_index,index2);
    output("+");
    movePointer(index2,empty_index);
    output("-]");
    movePointer(empty_index,index2);

    int n = v1->idegit;
    movePointer(v2->location+index2,v1->location+index1);
    for (int i = 0;i < n;++i) output("[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");

    movePointer(index1,0);
    movePointer(v1->location,0);
    output("\nend copy char to int\n");
}

void copyIntToChar(Variable *v1,Variable *v2,int index1,int index2,int empty_index) {
    output("\ncopy int to char\n");
    movePointer(0,v2->location);
    movePointer(0,v2->unit_size*(v2->fdegit+v2->idegit));
    movePointer(0,index2);
    for (int i = v2->idegit-1;i >= 0;--i) {
        movePointer(v2->unit_size,0);
        movePointer(v2->location+v2->unit_size*(v2->fdegit+i)+index2,v1->location+index1);
        output("[>+<-]>[<++++++++++>-]<");
        movePointer(v1->location+index1,v2->location+v2->unit_size*(v2->fdegit+i)+index2);
        output("[");

        movePointer(index2,empty_index);
        output("+");
        movePointer(empty_index,index2);

        movePointer(v2->location+v2->unit_size*(v2->fdegit+i)+index2,v1->location+index1);
        output("+");
        movePointer(v1->location+index1,v2->location+v2->unit_size*(v2->fdegit+i)+index2);
        output("-]");

        movePointer(index2,empty_index);
        output("[");
        movePointer(empty_index,index2);
        output("+");
        movePointer(index2,empty_index);
        output("-]");
        movePointer(empty_index,index2);
    }
    movePointer(v2->unit_size*(v2->fdegit),0);
    movePointer(index2,0);
    movePointer(v2->location,0);
    output("\nend copy int to char\n");
}

void clear(Variable *v,int start_unit,int index,int len) {
    output("\nclear\n");
    movePointer(0,v->location);
    movePointer(0,start_unit*v->unit_size+index);
    for (int i = 0;i < len;++i) {
        output("[-]");
        movePointer(0,v->unit_size);
    }
    movePointer(v->unit_size*(start_unit+len)+index,0);
    movePointer(v->location,0);
    output("\nend clear\n");
}

void printStr(int used_memory,char *str) {
    output("\nprint str\n");
    movePointer(0,used_memory);
    for (int i = 0;i < strlen(str);++i) {
        for (int j = 0;j < str[i];++j) output("+");
        output(".");
        for (int j = 0;j < str[i];++j) output("-");
    }
    movePointer(used_memory,0);
    output("\nend print str\n");
}

void printChar(Variable *v) {
    output("\nprint char\n");
    movePointer(0,v->location);
    output(".");
    movePointer(v->location,0);
    output("\nend print char\n");
}

void printUint(Variable *v) {
    output("\nprint uint\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*size(v));
    for (int i = 0;i < size(v)-1;++i) {
        movePointer(v->unit_size,0);
        output("[>>+<]>[<]>[<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------<+>>>[-]]<<");
    }
    output("<[-]>");
    movePointer(v->unit_size,0);
    output("++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    movePointer(v->location,0);
    output("\nend print uint\n");
}

void printInt(Variable *v) {
    output("\nprint int\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*(size(v)-1));
    output("[>>+<]>[<]>[++++++++++++++++++++++++++++++++++++++++++++.---------------------------------------------]<<");
    for (int i = 0;i < v->idegit-1;++i) {
        movePointer(v->unit_size,0);
        output("[>>+<]>[<]>[<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------<+>>>[-]]<<");
    }
    output("<[-]>");
    movePointer(v->unit_size,0);
    output("++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    movePointer(v->location,0);
    output("\nend print int\n");
}

void printFixed(Variable *v) {
    output("\nprint fixed\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*(size(v)-1));
    output("[>>+<]>[<]>[++++++++++++++++++++++++++++++++++++++++++++.---------------------------------------------]<<");
    for (int i = 0;i < v->idegit-1;++i) {
        movePointer(v->unit_size,0);
        output("[>>+<]>[<]>[<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------<+>>>[-]]<<");
    }
    output("<[-]>");
    movePointer(v->unit_size,0);
    output("++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    output(">++++++++++++++++++++++++++++++++++++++++++++++.----------------------------------------------<");
    for (int i = 0;i < v->fdegit;++i) {
        movePointer(v->unit_size,0);
        output("++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    }
    movePointer(v->location,0);
    output("\nend print fixed\n");
}

void scanUint(Variable *v) {
    output("\nscan uint\n");
    movePointer(0,v->location);
    output(">>+[-<,----------[----------------------[----------------<");
    for (int i = 0;i < v->idegit-1;++i) output(">>>");
    for (int i = 0;i < v->idegit-1;++i) output("<<<[>>>+<<<-]");
    output(">[<+>-]>+<]]>]<<");
    movePointer(v->location,0);
    output("\nend scan uint\n");
}

void scanInt(Variable *v) {
    output("\nscan int\n");
    movePointer(0,v->location);

    output(",---------------------------------------------[--->>+<]>[<]>-[+");
    for (int i = 0;i < v->idegit-1;++i) output(">>>");
    output(">+<");
    for (int i = 0;i < v->idegit-1;++i) output("<<<");

    output("]+[-<,----------[----------------------[----------------<");
    for (int i = 0;i < v->idegit-1;++i) output(">>>");
    for (int i = 0;i < v->idegit-1;++i) output("<<<[>>>+<<<-]");
    output(">[<+>-]>+<]]>]<<");
    movePointer(v->location,0);
    output("\nend scan int\n");
}

void scanFixed(Variable *v) {
    output("\nscan fixed\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*v->fdegit);

    output(",---------------------------------------------[--->>+<]>[<]>-[+");
    for (int i = 0;i < v->idegit-1;++i) output(">>>");
    output(">+<");
    for (int i = 0;i < v->idegit-1;++i) output("<<<");

    output("]+[-<,----------------------------------------------[--<");
    for (int i = 0;i < v->idegit-1;++i) output(">>>");
    for (int i = 0;i < v->idegit-1;++i) output("<<<[>>>+<<<-]");
    output(">[<+>-]>+<]>]+");
    for (int i = 0;i < v->fdegit-1;++i) output("[-<,----------[----------------------[----------------[<<<<+>>>>-]<<+>>]]>]<<<");
    output("[-<,----------[----------------------[----------------[<<<<+>>>>-],----------[----------------------]]]>]<<<<<");
    movePointer(v->location,0);
    output("\nend scan fixed\n");
}

void scanChar(Variable *v) {
    output("\nscan char\n");
    movePointer(0,v->location);
    output(",");
    movePointer(v->location,0);
    output("\nend scan char\n");
}

void ifBegin(Variable *v) {
    output("\nif\n");
    movePointer(0,v->location);
    output("[-");
    movePointer(v->location,0);
}

void ifEnd(Variable *v) {
    movePointer(0,v->location);
    output("]");
    movePointer(v->location,0);
    output("\nend if\n");
}

void ifElseBegin(Variable *v) {
    output("\nif\n");
    movePointer(0,v->location);
    output(">+<[->-<");
    movePointer(v->location,0);
}

void ifElseMid(Variable *v) {
    movePointer(0,v->location);
    output("]");
    output("\nelse\n");
    output(">[-<");
    movePointer(v->location,0);

}

void ifElseEnd(Variable *v) {
    movePointer(0,v->location);
    output(">]<");
    movePointer(v->location,0);
    output("\nend if\n");
}

void whileBegin(Variable *v) {
    output("\nwhile\n");
    movePointer(0,v->location);
    output("+[-");
    movePointer(v->location,0);
}

void whileMid(Variable *v) {
    output("\nwhile mid\n");
    movePointer(0,v->location);
    output(">[-<");
    movePointer(v->location,0);
}

void whileEnd(Variable *v) {
    movePointer(0,v->location);
    output(">]<]");
    movePointer(v->location,0);
    output("\nend while\n");
}

void equalUnsigned(Variable *v) {
    output("\nequal unsigned\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output("[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">>+>[<->-]<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend equal unsigned\n");
}

void notEqualUnsigned(Variable *v) {
    output("\nnot equal unsigned\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output("[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend not equal unsigned\n");
}

void lessUnsigned(Variable *v) {
    output("\nless unsigned\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend less unsigned\n");
}

void greaterEqualUnsigned(Variable *v) {
    output("\ngreater equal unsigned\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    output(">+<[>-<-]");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend greater equal unsigned\n");
}

void equalSigned(Variable *v) {
    output("\nequal signed\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("[>-<-]>>+<[>-<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">-]>[<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">>-]+>[<->-]<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend equal signed\n");
}

void notEqualSigned(Variable *v) {
    output("\nnot equal signed\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("[>-<-]>>+<[>-<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">-]>[<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">>-]<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend not equal signed\n");
}

void lessSigned(Variable *v) {
    output("\nless signed\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("[>>+<<-]>[>++<-]>[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    output(">>>>]<<-[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[-]>[-]>>>>>>>");
    output(">>>>]<<-[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">>>[<<<+>>>-]>]<<-[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("+");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    output(">+<[>-<-]>[<+>-]>>>]<<[+]<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend less signed\n");
}

void greaterEqualSigned(Variable *v) {
    output("\ngreater equal signed\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) output(">>>>>>>>");
    output("[>>+<<-]>[>++<-]>[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    output(">>>>]<<-[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[-]>[-]>>>>>>>");
    output(">>>>]<<-[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    for (int i = 0;i < n;++i) output("[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    output(">>>[<<<+>>>-]>]<<-[>>+<]>[<]>-[+<<<<");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    output("+");
    for (int i = 0;i < n;++i) output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    output(">+<[>-<-]>[<+>-]>>>]<<[+]<+<[>-<-]");
    for (int i = 0;i < n;++i) output("<<<<<<<<");
    movePointer(v->location,0);
    output("\nend greater equal signed\n");
}

void equalChar(Variable *v) {
    output("\nequal char\n");
    movePointer(0,v->location);
    output("[>-<-]>[>>>>>>>>>>+<<<<<<<<<<[-]]>>>>>>>>>+>[<->-]<<<<<<<<<<<");
    movePointer(v->location,0);
    output("\nend equal char\n");
}

void notEqualChar(Variable *v) {
    output("\nnot equal char\n");
    movePointer(0,v->location);
    output("[>-<-]>[>>>>>>>>>>+<<<<<<<<<<[-]]<");
    movePointer(v->location,0);
    output("\nend not equal char\n");
}

void lessChar(Variable *v) {
    output("\nless char\n");
    movePointer(0,v->location);
    output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<<[-]>+>>]<<-<]>[-]<");
    movePointer(v->location,0);
    output("\nend less char\n");
}

void greaterEqualChar(Variable *v) {
    output("\ngreater equal char\n");
    movePointer(0,v->location);
    output("[->[>>+<]>[<]>-[+>>>>>+<<<<<<<<[-]>+>>]<<-<]>[-]>>>>>>>>+<[>-<-]<<<<<<<<");
    movePointer(v->location,0);
    output("\nend greater equal char\n");
}
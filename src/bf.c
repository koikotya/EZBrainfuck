#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
// #include "generator.h"

extern FILE *Fresult;

void movePointer(int p1,int p2) {
    if (p1 < p2) {
        for (int i = 0;i < p2-p1;++i) fprintf(Fresult,">");
    } else {
        for (int i = 0;i < p1-p2;++i) fprintf(Fresult,"<");
    }
}

void setSign(Variable* v,int index,bool sign) {
    if (sign == false) return;
    fprintf(Fresult,"\nset sign\n");
    movePointer(0,v->location+v->unit_size*(v->fdegit+v->idegit)+index);
    fprintf(Fresult,"+");
    movePointer(v->location+v->unit_size*(v->fdegit+v->idegit)+index,0);
    fprintf(Fresult,"\nend set sign\n");
}

void turnSign(Variable* v,int index,bool sign) {
    if (sign == false) return;
    fprintf(Fresult,"\nturn sign\n");
    movePointer(0,v->location+v->unit_size*(v->fdegit+v->idegit)+index);
    fprintf(Fresult,">+<[>-<-]>[<+>-]<");
    movePointer(v->location+v->unit_size*(v->fdegit+v->idegit)+index,0);
    fprintf(Fresult,"\nend turn sign\n");
}

void setChar(Variable* v,int index,char *literal) {
    fprintf(Fresult,"\nset char\n");
    int num = atoi(literal);
    movePointer(0,v->location+index);
    for (int i = 0;i < num;++i) fprintf(Fresult,"+");
    movePointer(v->location+index,0);
    fprintf(Fresult,"\nend set char\n");
}

void setInteger(Variable* v,int index,char *literal) {
    fprintf(Fresult,"\nset integer\n");
    int len = strlen(literal);
    movePointer(0,v->location+index);
    for (int i = len-1;i >= 0;--i) {
        for (int j = 0;j < literal[i]-'0';++j) fprintf(Fresult,"+");
        movePointer(0,v->unit_size);
    }
    movePointer(v->location+index+v->unit_size*len,0);
    fprintf(Fresult,"\nend set integer\n");
}

void setDecimal(Variable* v,int index,char *literal,Operation op) {
    fprintf(Fresult,"\nset decimal\n");
    if (op == INT_LITERAL) {
        int len = strlen(literal);
        movePointer(0,v->location+v->unit_size*v->fdegit+index);
        for (int i = len-1;i >= 0;--i) {
            for (int j = 0;j < literal[i]-'0';++j) fprintf(Fresult,"+");
            movePointer(0,v->unit_size);
        }
        movePointer(v->location+v->unit_size*(v->fdegit+len)+index,0);
    } else if (op == DECIMAL_LITERAL) {
        char str1[256],str2[256];
        sscanf(literal,"%[0-9].%[0-9]",str1,str2);
        int len1 = strlen(str1),len2 = strlen(str2);
        movePointer(0,v->location+v->unit_size*v->fdegit+index);
        for (int i = len1-1;i >= 0;--i) {
            for (int j = 0;j < str1[i]-'0';++j) fprintf(Fresult,"+");
            movePointer(0,v->unit_size);
        }
        movePointer(v->unit_size*len1,0);
        for (int i = 0;i < len2;++i) {
            movePointer(v->unit_size,0);
            for (int j = 0;j < str2[i]-'0';++j) fprintf(Fresult,"+");
        }
        movePointer(0,v->unit_size*len2);
        movePointer(v->location+v->unit_size*v->fdegit+index,0);
    } else {
        printf("error setDecimal\n");
    }
    fprintf(Fresult,"\nend set decimal\n");
}

// todo:add関数その他にVriable構造体を渡すように改造
void add(int n) {
    fprintf(Fresult,"\nadd\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"\nend add\n");
}

void sub(int n) {
    fprintf(Fresult,"\nsub\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"\nend sub\n");
}

void signedAdd(int n) {
    fprintf(Fresult,"\nsigned add\n");
    movePointer(0,n*8);
    fprintf(Fresult,"[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[+[-]<<<");
    movePointer(n*8,0);
    sub(n);
    movePointer(0,n*8);
    fprintf(Fresult,"[->[>-<-]>+[<+>-]<<");

    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<+++++++++<[>-<-]<");
    fprintf(Fresult,">+");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>");
    
    fprintf(Fresult,"<]>>>>-<]>[-<<<<");
    movePointer(n*8,0);
    add(n);
    movePointer(0,n*8);
    fprintf(Fresult,">>>>]<<<<");
    movePointer(n*8,0);
    fprintf(Fresult,"\nend signed add\n");
}

void signedSub(int n) {
    fprintf(Fresult,"\nigned sub\n");
    movePointer(0,n*8);
    fprintf(Fresult,"[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[+[-]<<<");
    movePointer(n*8,0);
    add(n);
    movePointer(0,n*8);
    fprintf(Fresult,">>>>-<]>[-<<<<");
    movePointer(n*8,0);
    sub(n);
    movePointer(0,n*8);

    fprintf(Fresult,"[->[>-<-]>+[<+>-]<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<+++++++++<[>-<-]<");
    fprintf(Fresult,">+");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>");
    fprintf(Fresult,"<]>>>>]<<<<");

    // fprintf(Fresult,"[->[>-<-]>+[<+>-]<<]>>>>]<<<<");

    movePointer(n*8,0);
    fprintf(Fresult,"\nend signed sub\n");
}

void charAdd() {
    fprintf(Fresult,"\nchar add\n");
    fprintf(Fresult,"[>+<-]");
    fprintf(Fresult,"\nend char add\n");
}

void charSub() {
    fprintf(Fresult,"\nchar sub\n");
    fprintf(Fresult,"[>-<-]");
    fprintf(Fresult,"\nend char sub\n");
}

void mult(int n) {
    fprintf(Fresult,"\nmult\n");
    for (int i = 0;i < n;++i) {
        fprintf(Fresult,"[->");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[>+>+<<-]>[<+>-]>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,">>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"<<<]");
        for (int j = 0;j < n;++j) fprintf(Fresult,">>>>>>>>");
        fprintf(Fresult,">");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
        fprintf(Fresult,">>>>>>>");
    }
    fprintf(Fresult,">");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[-]>>>>>>>>");
    fprintf(Fresult,"<<<<<<[>+<-]");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<<<<<<<<<");
    fprintf(Fresult,">>>>>");
    fprintf(Fresult,"\nend mult\n");
}

void signedMult(int n) {
    fprintf(Fresult,"\nsigned mult\n");
    for (int i = 0;i < n;++i) {
        fprintf(Fresult,"[->");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[>+>+<<-]>[<+>-]>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,">>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"<<<]");
        for (int j = 0;j < n;++j) fprintf(Fresult,">>>>>>>>");
        fprintf(Fresult,">");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
        fprintf(Fresult,">>>>>>>");
    }
    fprintf(Fresult,">");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[-]>>>>>>>>");
    fprintf(Fresult,"<<<<<<[>+<-]>>>>>[>-<-]>[+[-]>>>+<<<]<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<<<<<<<<<");
    fprintf(Fresult,"\nend signed mult\n");
}

void multShort(int n) {
    fprintf(Fresult,"\nmult short\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>+>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,">>>>>>>[-<<<<<<<[->");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[>+>+<<-]>[<+>-]>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,">>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"<<<]>");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
    fprintf(Fresult,">>>>>>>>>>>>>>]");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    fprintf(Fresult,"<<<<<<<<<<<<[>+<-]<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[-]<<<<<<<<");
    fprintf(Fresult,">>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"\nend mult short\n");
}

void signedMultShort(int n) {
    fprintf(Fresult,"\nmult short\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>+>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,">>>>>>>[-<<<<<<<[->");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[>+>+<<-]>[<+>-]>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,">>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"<<<]>");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<[>>>>>>>>+<<<<<<<<-]");
    fprintf(Fresult,">>>>>>>>>>>>>>]");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    fprintf(Fresult,"<<<<<<<<<<<<[>+<-]>>>>>[>-<-]>[+[-]>>>+<<<]<<<<<<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[-]<<<<<<<<");
    fprintf(Fresult,">>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"\nend mult short\n");
}

void divide(int n) {
    /*
    a/b = q..r
    0: b
    1: a,r
    2: 
    3: temp copy b
    4: 引き算用カウンタ
    5: temp copy a
    6: ループ用フラグ
    7: q 
    */
    fprintf(Fresult,"\ndivide\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    for (int i = 0;i < n;++i) {
        fprintf(Fresult,"<<<<<<<<[");
        for (int j = 0;j < n;++j) fprintf(Fresult,">>>>>>>>");
        fprintf(Fresult,"+");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"-]");
    }
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    for (int i = 0;i < n;++i) {
        for (int j = 0;j < n;++j) fprintf(Fresult,"[<<<<<<<<+>>>>>>>>-]>>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"<<+[<<<<<");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[>+>>>+<<<<-]>[<+>-]>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"<");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[>>+>>+<<<<-]>>[<<+>>-]>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,">>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[-<<<[>>+<]>[<]>-[+>>>>>>>>>+<<<<<<<<<<<++++++++++>>]<<->>>]>>>>>>>>");
        fprintf(Fresult,"[<<<");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<[-]");
        fprintf(Fresult,">>>>>->-<<<<<<");
        for (int j = 0;j < n;++j) fprintf(Fresult,">>>>[<<<<+>>>>-]>>>>");
        fprintf(Fresult,">>>-]");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<[-]<");
        fprintf(Fresult,">>>+<]<<<<<<");
    }
    fprintf(Fresult,"\nend divide\n");
}

void signedDivide(int n) {
    fprintf(Fresult,"\nsigned divide\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    for (int i = 0;i < n;++i) {
        fprintf(Fresult,"<<<<<<<<[");
        for (int j = 0;j < n;++j) fprintf(Fresult,">>>>>>>>");
        fprintf(Fresult,"+");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"-]");
    }
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    for (int i = 0;i < n;++i) {
        for (int j = 0;j < n;++j) fprintf(Fresult,"[<<<<<<<<+>>>>>>>>-]>>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"<<+[<<<<<");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[>+>>>+<<<<-]>[<+>-]>>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,"<");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[>>+>>+<<<<-]>>[<<+>>-]>>>>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<");
        fprintf(Fresult,">>>>");
        for (int j = 0;j < n;++j) fprintf(Fresult,"[-<<<[>>+<]>[<]>-[+>>>>>>>>>+<<<<<<<<<<<++++++++++>>]<<->>>]>>>>>>>>");
        fprintf(Fresult,"[<<<");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<<[-]");
        fprintf(Fresult,">>>>>->-<<<<<<");
        for (int j = 0;j < n;++j) fprintf(Fresult,">>>>[<<<<+>>>>-]>>>>");
        fprintf(Fresult,">>>-]");
        for (int j = 0;j < n;++j) fprintf(Fresult,"<<<<<<<[-]<");
        fprintf(Fresult,">>>+<]<<<<<<");
    }
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>>>>>>>>>");
    fprintf(Fresult,"[>>>+<<<-]>[>+>-<<-]>[<+>-]>[+[-]>>>>+<<<<]<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<<<<<<<<<");
    fprintf(Fresult,"\nend signed divide\n");
}

// Unidenさんありがとうございます
void divide_Uniden(int n) {
    fprintf(Fresult,"\ndivide Uniden\n");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>+>");
    fprintf(Fresult,"<[<<<<<<<[>>>>>>>[-]>>>>>>>>[>>>>>>>>]");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"[->>>>>>>>]+[->>>>>>>>[>>>>>>>>]<<<<<<<+<[<<<<<<<<]>>>>>>>>]<<<<<<<-[");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<([->>>>>>>>+<<<<<<<<]");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    fprintf(Fresult,">>+<<-]>>+[<+[<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->[>>+<]>[<]>-[+>>>>>+>>>>-<<<<<<<<<<<++++++++++>>]>+<<<-<]>>>>[-<<<<+>>>>]>>>>");
    fprintf(Fresult,"[->-<]>>>+>[+]<<<<>]<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"[-> ---------[>>+<]>[<]>-[+<<---------->> >>>>>+>>>>-<<<<<<<<<]>+< << ++++++++++<] >>>>[-<<<<+>>>>]>>>>");
    fprintf(Fresult,"->>> >[+]<-[->>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"+");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>");
    fprintf(Fresult,"<<<<]<->>+<<[>]>[<<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<[->>>>>>>>+<<<<<<<<]<<<<<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,">>>>>>>>[-<<<<<<<<+>>>>>>>>]");
    fprintf(Fresult,">>>]>-<<]]<]<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    fprintf(Fresult,"\nend divide Uniden\n");
}

// v2をv1にコピー
// 0番地からスタート
void move(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len) {
    fprintf(Fresult,"\nmove\n");
    movePointer(0,v2->location);
    movePointer(0,start_unit2*v2->unit_size+index2);
    for (int i = 0;i < len;++i) {
        fprintf(Fresult,"[");
        movePointer(v2->location+v2->unit_size*(start_unit2+i)+index2,v1->location+v1->unit_size*(start_unit1+i)+index1);
        fprintf(Fresult,"+");
        movePointer(v1->location+v1->unit_size*(start_unit1+i)+index1,v2->location+v2->unit_size*(start_unit2+i)+index2);
        fprintf(Fresult,"-]");
        movePointer(0,v2->unit_size);
    }
    movePointer(v2->unit_size*(start_unit2+len)+index2,0);
    movePointer(v2->location,0);
    fprintf(Fresult,"\nend move\n");
}

void copy(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len,int empty_index) {
    fprintf(Fresult,"\ncopy\n");
    movePointer(0,v2->location);
    movePointer(0,start_unit2*v2->unit_size+index2);
    for (int i = 0;i < len;++i) {
        fprintf(Fresult,"[");

        movePointer(index2,empty_index);
        fprintf(Fresult,"+");
        movePointer(empty_index,index2);

        movePointer(v2->location+v2->unit_size*(start_unit2+i)+index2,v1->location+v1->unit_size*(start_unit1+i)+index1);
        fprintf(Fresult,"+");
        movePointer(v1->location+v1->unit_size*(start_unit1+i)+index1,v2->location+v2->unit_size*(start_unit2+i)+index2);
        fprintf(Fresult,"-]");

        movePointer(index2,empty_index);
        fprintf(Fresult,"[");
        movePointer(empty_index,index2);
        fprintf(Fresult,"+");
        movePointer(index2,empty_index);
        fprintf(Fresult,"-]");
        movePointer(empty_index,index2);

        movePointer(0,v2->unit_size);
    }
    movePointer(v2->unit_size*(start_unit2+len)+index2,0);
    movePointer(v2->location,0);
    fprintf(Fresult,"\nend copy\n");
}

void clear(Variable *v,int start_unit,int index,int len) {
    fprintf(Fresult,"\nclear\n");
    movePointer(0,v->location);
    movePointer(0,start_unit*v->unit_size+index);
    for (int i = 0;i < len;++i) {
        fprintf(Fresult,"[-]");
        movePointer(0,v->unit_size);
    }
    movePointer(v->unit_size*(start_unit+len)+index,0);
    movePointer(v->location,0);
    fprintf(Fresult,"\nend clear\n");
}

void printStr(int used_memory,char *str) {
    fprintf(Fresult,"\nprint str\n");
    movePointer(0,used_memory);
    for (int i = 0;i < strlen(str);++i) {
        for (int j = 0;j < str[i];++j) fprintf(Fresult,"+");
        fprintf(Fresult,".");
        for (int j = 0;j < str[i];++j) fprintf(Fresult,"-");
    }
    movePointer(used_memory,0);
    fprintf(Fresult,"\nend print str\n");
}

void printChar(Variable *v) {
    fprintf(Fresult,"\nprint char\n");
    movePointer(0,v->location);
    fprintf(Fresult,".");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend print char\n");
}

void printUint(Variable *v) {
    fprintf(Fresult,"\nprint uint\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*size(v));
    for (int i = 0;i < size(v)-1;++i) {
        movePointer(v->unit_size,0);
        fprintf(Fresult,"[>>+<]>[<]>[<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------<+>>>[-]]<<");
    }
    fprintf(Fresult,"<[-]>");
    movePointer(v->unit_size,0);
    fprintf(Fresult,"++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend print uint\n");
}

void printInt(Variable *v) {
    fprintf(Fresult,"\nprint int\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*(size(v)-1));
    fprintf(Fresult,"[>>+<]>[<]>[++++++++++++++++++++++++++++++++++++++++++++.---------------------------------------------]<<");
    for (int i = 0;i < v->idegit-1;++i) {
        movePointer(v->unit_size,0);
        fprintf(Fresult,"[>>+<]>[<]>[<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------<+>>>[-]]<<");
    }
    fprintf(Fresult,"<[-]>");
    movePointer(v->unit_size,0);
    fprintf(Fresult,"++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend print int\n");
}

void printFixed(Variable *v) {
    fprintf(Fresult,"\nprint fixed\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*(size(v)-1));
    fprintf(Fresult,"[>>+<]>[<]>[++++++++++++++++++++++++++++++++++++++++++++.---------------------------------------------]<<");
    for (int i = 0;i < v->idegit-1;++i) {
        movePointer(v->unit_size,0);
        fprintf(Fresult,"[>>+<]>[<]>[<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------<+>>>[-]]<<");
    }
    fprintf(Fresult,"<[-]>");
    movePointer(v->unit_size,0);
    fprintf(Fresult,"++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    fprintf(Fresult,">++++++++++++++++++++++++++++++++++++++++++++++.----------------------------------------------<");
    for (int i = 0;i < v->fdegit;++i) {
        movePointer(v->unit_size,0);
        fprintf(Fresult,"++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------");
    }
    movePointer(v->location,0);
    fprintf(Fresult,"\nend print fixed\n");
}

void scanUint(Variable *v) {
    fprintf(Fresult,"\nscan uint\n");
    movePointer(0,v->location);
    fprintf(Fresult,">>+[-<,----------[----------------------[----------------<");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,">>>");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,"<<<[>>>+<<<-]");
    fprintf(Fresult,">[<+>-]>+<]]>]<<");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend scan uint\n");
}

void scanInt(Variable *v) {
    fprintf(Fresult,"\nscan int\n");
    movePointer(0,v->location);

    fprintf(Fresult,",---------------------------------------------[--->>+<]>[<]>-[+");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,">>>");
    fprintf(Fresult,">+<");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,"<<<");

    fprintf(Fresult,"]+[-<,----------[----------------------[----------------<");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,">>>");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,"<<<[>>>+<<<-]");
    fprintf(Fresult,">[<+>-]>+<]]>]<<");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend scan int\n");
}

void scanFixed(Variable *v) {
    fprintf(Fresult,"\nscan fixed\n");
    movePointer(0,v->location);
    movePointer(0,v->unit_size*v->fdegit);

    fprintf(Fresult,",---------------------------------------------[--->>+<]>[<]>-[+");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,">>>");
    fprintf(Fresult,">+<");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,"<<<");

    fprintf(Fresult,"]+[-<,----------------------------------------------[--<");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,">>>");
    for (int i = 0;i < v->idegit-1;++i) fprintf(Fresult,"<<<[>>>+<<<-]");
    fprintf(Fresult,">[<+>-]>+<]>]+");
    for (int i = 0;i < v->fdegit-1;++i) fprintf(Fresult,"[-<,----------[----------------------[----------------[<<<<+>>>>-]<<+>>]]>]<<<");
    fprintf(Fresult,"[-<,----------[----------------------[----------------[<<<<+>>>>-],----------[----------------------]]]>]<<<<<");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend scan fixed\n");
}

void scanChar(Variable *v) {
    fprintf(Fresult,"\nscan char\n");
    movePointer(0,v->location);
    fprintf(Fresult,",");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend scan char\n");
}

void ifBegin(Variable *v) {
    fprintf(Fresult,"\nif\n");
    movePointer(0,v->location);
    fprintf(Fresult,"[-");
    movePointer(v->location,0);
}

void ifEnd(Variable *v) {
    movePointer(0,v->location);
    fprintf(Fresult,"]");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend if\n");
}

void ifElseBegin(Variable *v) {
    fprintf(Fresult,"\nif\n");
    movePointer(0,v->location);
    fprintf(Fresult,">+<[->-<");
    movePointer(v->location,0);
}

void ifElseMid(Variable *v) {
    movePointer(0,v->location);
    fprintf(Fresult,"]");
    fprintf(Fresult,"\nelse\n");
    fprintf(Fresult,">[-<");
    movePointer(v->location,0);

}

void ifElseEnd(Variable *v) {
    movePointer(0,v->location);
    fprintf(Fresult,">]<");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend if\n");
}

void whileBegin(Variable *v) {
    fprintf(Fresult,"\nwhile\n");
    movePointer(0,v->location);
    fprintf(Fresult,"+[-");
    movePointer(v->location,0);
}

void whileMid(Variable *v) {
    fprintf(Fresult,"\nwhile mid\n");
    movePointer(0,v->location);
    fprintf(Fresult,">[-<");
    movePointer(v->location,0);
}

void whileEnd(Variable *v) {
    movePointer(0,v->location);
    fprintf(Fresult,">]<]");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend while\n");
}

void equalUnsigned(Variable *v) {
    fprintf(Fresult,"\nequal unsigned\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>");
    fprintf(Fresult,">>+>[<->-]<<<");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend equal unsigned\n");
}

void lessUnsigned(Variable *v) {
    fprintf(Fresult,"\nless unsigned\n");
    movePointer(0,v->location);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) fprintf(Fresult,"[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>");
    for (int i = 0;i < n;++i) fprintf(Fresult,"<<<<<<<<");
    movePointer(v->location,0);
    fprintf(Fresult,"\nend less unsigned\n");
}
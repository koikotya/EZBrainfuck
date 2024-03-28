#include <algorithm>
#include <string>

#include "attribute.hpp"
#include "bf.hpp"

using std::string;
using std::min;

string setLiteral(Attribute *v1,Attribute *v2,int index1) {
    string res = "";
    if (v1->type == UINT_TYPE) {
        res += setInteger(v1,index1,v2->str);
    } else if (v1->type == INT_TYPE) {
        res += setInteger(v1,index1,v2->str);
        res += setSign(v1,index1,v2->negative);
    } else if (v1->type == FIXED_TYPE) {
        res += setDecimal(v1,index1,v2);
        res += setSign(v1,index1,v2->negative);
    } else if (v1->type == CHAR_TYPE) {
        res += setChar(v1,index1,v2->str);
    } else if (v1->type == BOOL_TYPE) {
        res += setChar(v1,index1,v2->str);
    } else {
        // printErr("error setLiteral\n");
    }
    return res;
}

// v2をv1のメモリに書き込む
// v1のどのindexに書き込みかは引数で指定
string setValue(Attribute *v1,Attribute *v2,int index1) {
    string res = "";
    int v2_index = 1;
    if (v2->op == INT_LITERAL || v2->op == DECIMAL_LITERAL) {
        // v2がリテラルの場合
        res += setLiteral(v1,v2,index1);
    } else if (v2->op == PLUS_OP || v2->op == MINUS_OP || v2->op == TIMES_OP || v2->op == DIVIDE_OP || v2->op == MOD_OP) {
        // v2が計算中の値の場合
        // v2をv1にムーブ
        v2_index = getIndex(v2->op);
        int fdegit = min(v1->fdegit,v2->fdegit);
        int idegit = min(v1->idegit,v2->idegit);
        if (v1->type == CHAR_TYPE && CHAR_TYPE < v2->type) {
            Attribute *v3;
            v3 = (Attribute *)malloc(sizeof(Attribute));
            Attribute::allocateVar(v3,0);
            res += moveIntToChar(v3,v2,1,v2_index);
            res += move(v1,v3,0,0,index1,1,1);
            Attribute::deallocVar(v3);
        } else if (v2->type == CHAR_TYPE && CHAR_TYPE < v1->type) {
            Attribute *v3;
            v3 = (Attribute *)malloc(sizeof(Attribute));
            v3->idegit = 3;
            Attribute::allocateVar(v3,0);
            res += moveCharToInt(v3,v2,1,v2_index);
            res += move(v1,v3,v1->fdegit,0,index1,2,min(v1->idegit,v3->idegit));
            res += clear(v3,0,1,3);
            Attribute::deallocVar(v3);
        } else {
            res += move(v1,v2,v1->fdegit-fdegit,v2->fdegit-fdegit,index1,v2_index,fdegit+idegit);
            // 符号の処理
            if ((v1->type == INT_TYPE || v1->type == FIXED_TYPE) && (v2->type == INT_TYPE || v2->type == FIXED_TYPE)) {
                res += move(v1,v2,v1->size()-1,v2->size()-1,index1,v2_index,1);
            }
            res += turnSign(v1,index1,v2->negative);
        }
        res += clear(v2,0,v2_index,v2->size());
        Attribute::deallocVar(v2);
    } else {
        // v2が変数の場合
        // v2をv1にコピー
        int fdegit = min(v1->fdegit,v2->fdegit);
        int idegit = min(v1->idegit,v2->idegit);
        if (v1->type == CHAR_TYPE && CHAR_TYPE < v2->type) {
            Attribute *v3;
            v3 = (Attribute *)malloc(sizeof(Attribute));
            Attribute::allocateVar(v3,0);
            res += copyIntToChar(v3,v2,1,v2_index,2);
            res += move(v1,v3,0,0,index1,1,1);
            Attribute::deallocVar(v3);
        } else if (v2->type == CHAR_TYPE && CHAR_TYPE < v1->type) {
            Attribute *v3;
            v3 = (Attribute *)malloc(sizeof(Attribute));
            v3->idegit = 3;
            Attribute::allocateVar(v3,0);
            res += copyCharToInt(v3,v2,1,v2_index,2);
            res += move(v1,v3,v1->fdegit,0,index1,2,min(v1->idegit,v3->idegit));
            res += clear(v3,0,1,3);
            Attribute::deallocVar(v3);
        } else {
            res += copy(v1,v2,v1->fdegit-fdegit,v2->fdegit-fdegit,index1,v2_index,fdegit+idegit,2);
            // 符号の処理
            if ((v1->type == INT_TYPE || v1->type == FIXED_TYPE) && (v2->type == INT_TYPE || v2->type == FIXED_TYPE)) {
                res += copy(v1,v2,v1->size()-1,v2->size()-1,index1,v2_index,1,2);
            }
            res += turnSign(v1,index1,v2->negative);
        }
    }
    return res;
}

string movePointer(int p1,int p2) {
    string res = "";
    if (p1 < p2) {
        for (int i = 0;i < p2-p1;++i) res += ">";
    } else {
        for (int i = 0;i < p1-p2;++i) res += "<";
    }
    return res;
}

string setSign(Attribute* v,int index,bool sign) {
    string res = "";
    if (sign == false) return res;
    res += "\nset sign\n";
    res += movePointer(0,v->address+INTERVAL*(v->fdegit+v->idegit)+index);
    res += "+";
    res += movePointer(v->address+INTERVAL*(v->fdegit+v->idegit)+index,0);
    res += "\nend set sign\n";
    return res;
}

string turnSign(Attribute* v,int index,bool sign) {
    string res = "";
    if (sign == false) return res;
    res += "\nturn sign\n";
    res += movePointer(0,v->address+INTERVAL*(v->fdegit+v->idegit)+index);
    res += ">+<[>-<-]>[<+>-]<";
    res += movePointer(v->address+INTERVAL*(v->fdegit+v->idegit)+index,0);
    res += "\nend turn sign\n";
    return res;
}

string setChar(Attribute* v,int index,string literal) {
    string res = "";
    res += "\nset char\n";
    int num = std::stoi(literal);
    res += movePointer(0,v->address+index);
    for (int i = 0;i < num;++i) res += "+";
    res += movePointer(v->address+index,0);
    res += "\nend set char\n";
    return res;
}

string setInteger(Attribute* v,int index,string literal) {
    string res = "";
    res += "\nset integer\n";
    int len = literal.size();
    res += movePointer(0,v->address+index);
    for (int i = len-1;i >= 0;--i) {
        for (int j = 0;j < literal[i]-'0';++j) res += "+";
        res += movePointer(0,INTERVAL);
    }
    res += movePointer(v->address+index+INTERVAL*len,0);
    res += "\nend set integer\n";
    return res;
}

string setDecimal(Attribute* v1,int index,Attribute* v2) {
    string res = "";
    res += "\nset decimal\n";
    if (v2->op == INT_LITERAL) {
        int len = v2->idegit;
        res += movePointer(0,v1->address+INTERVAL*v1->fdegit+index);
        for (int i = len-1;i >= 0;--i) {
            for (int j = 0;j < v2->str[i]-'0';++j) res += "+";
            res += movePointer(0,INTERVAL);
        }
        res += movePointer(v1->address+INTERVAL*(v1->fdegit+len)+index,0);
    } else if (v2->op == DECIMAL_LITERAL) {
        string str1 = v2->str.substr(0,v2->idegit),str2 = v2->str.substr(v2->idegit+1,v2->idegit);
        int len1 = v2->idegit,len2 = v2->fdegit;
        res += movePointer(0,v1->address+INTERVAL*v1->fdegit+index);
        for (int i = len1-1;i >= 0;--i) {
            for (int j = 0;j < str1[i]-'0';++j) res += "+";
            res += movePointer(0,INTERVAL);
        }
        res += movePointer(INTERVAL*len1,0);
        for (int i = 0;i < len2;++i) {
            res += movePointer(INTERVAL,0);
            for (int j = 0;j < str2[i]-'0';++j) res += "+";
        }
        res += movePointer(0,INTERVAL*len2);
        res += movePointer(v1->address+INTERVAL*v1->fdegit+index,0);
    } else {
        // printErr("error setDecimal\n");
    }
    res += "\nend set decimal\n";
    return res;
}

// todo:add関数その他にVriable構造体を渡すように改造
string add(int n) {
    string res = "";
    res += "\nadd\n";
    for (int i = 0;i < n;++i) res += ">[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "\nend add\n";
    return res;
}

string sub(int n) {
    string res = "";
    res += "\nsub\n";
    for (int i = 0;i < n;++i) res += ">[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "\nend sub\n";
    return res;
}

string addSigned(int n) {
    string res = "";
    res += "\nsigned add\n";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>>";
    res += " [>[>-<-]>+[<+>-]<";
    for (int i = 0;i < n;++i) res += "<<<<<<<+++++++++<[>-<-]";
    res += "+";
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>";
    res += "<[-]]>>>>-<+[-]]>[<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>";
    res += ">>>>-]<<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "\nend signed add\n";
    return res;
}

string subSigned(int n) {
    string res = "";
    res += "\nsigned sub\n";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">[>>>+<<<-]>[>+>-<<-]>[<+>-]>>+<[<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>";
    res += ">>>>-<+[-]]>[<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>>>>>>>>";
    res += "[>[>-<-]>+[<+>-]<";
    for (int i = 0;i < n;++i) res += "<<<<<<<+++++++++<[>-<-]";
    res += "+";
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>[<+>-]>>>>>>>";
    res += "<[-]]>>>>-]<<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "\nend signed sub\n";
    return res;
}

string addChar() {
    string res = "";
    res += "\nchar add\n";
    res += ">[>+<-]<";
    res += "\nend char add\n";
    return res;
}

string subChar() {
    string res = "";
    res += "\nchar sub\n";
    res += ">[>-<-]<";
    res += "\nend char sub\n";
    return res;
}

string mult(int n) {
    string res = "";
    res += "\nmult\n";
    for (int i = 0;i < n;++i) {
        res += ">[-";
        for (int i = 0;i < n;++i) res += ">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>";
        for (int i = 0;i < n;++i) res += "<<<<<<<<";
        res += "]>";
        for (int i = 0;i < n;++i) res += ">>>>>>>>";
        for (int i = 0;i < n;++i) res += "<<<<<<<<[>>>>>>>>+<<<<<<<<-]";
        res += ">>>>>>";
    }
    for (int i = 0;i < n;++i) res += ">>[-]>>>>>>";
    res += "<<<<[>+<-]>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<<<<<<<<<";
    res += "\nend mult\n";
    return res;
}

string multSigned(int n) {
    string res = "";
    res += "\nsigned mult\n";
    for (int i = 0;i < n;++i) {
        res += ">[-";
        for (int i = 0;i < n;++i) res += ">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>";
        for (int i = 0;i < n;++i) res += "<<<<<<<<";
        res += "]>";
        for (int i = 0;i < n;++i) res += ">>>>>>>>";
        for (int i = 0;i < n;++i) res += "<<<<<<<<[>>>>>>>>+<<<<<<<<-]";
        res += ">>>>>>";
    }
    for (int i = 0;i < n;++i) res += ">>[-]>>>>>>";
    res += "<<<<[>+<-]>>>>>[>-<-]>[+[-]>>>+<<<]<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<<<<<<<<<";
    res += "\nend signed mult\n";
    return res;
}

string multShort(int n) {
    string res = "";
    res += "\nmult short\n";
    for (int i = 0;i < n;++i) res += "+>>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "[->[-";
    for (int i = 0;i < n;++i) res += ">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "]>";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<[>>>>>>>>+<<<<<<<<-]";
    res += ">>>>>>]";
    for (int i = 0;i < n;++i) res += ">>[-]>>>>>>";
    res += "<<<<[>+<-]>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<<<<<<<<<";
    res += "\nend mult short\n";
    return res;
}

string multSignedShort(int n) {
    string res = "";
    res += "\nsigned mult short\n";
    for (int i = 0;i < n;++i) res += "+>>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "[->[-";
    for (int i = 0;i < n;++i) res += ">[>+>+<<-]>[<+>-]>[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "]>";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<[>>>>>>>>+<<<<<<<<-]";
    res += ">>>>>>]";
    for (int i = 0;i < n;++i) res += ">>[-]>>>>>>";
    res += "<<<<[>+<-]>>>>>[>-<-]>[+[-]>>>+<<<]<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<<<<<<<<<";
    res += "\nend signed mult short\n";
    return res;
}

string divide(int n) {
    string res = "";
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
    res += "\ndivide\n";
    // ｂを右にシフト
    for (int i = 0;i < n;++i) res += ">>>>>>>>+";
    res += "[-<<<<<<[";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += "+";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "-]<<]";
    // ループ用フラグを立てる
    for (int i = 0;i < n;++i) res += ">>>>>>>>+";
    // 除算
    res += "[";
    for (int i = 0;i < n;++i) res += ">>[<<<<<<<<+>>>>>>>>-]>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "[<<<<<<<+>";
    for (int i = 0;i < n;++i) res += "[>+>+<<-]>[<+>-]>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += ">>";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+<<++++++++++>>>>>>>+<<<<<]<<-<]>>>>>>>>";
    res += "[-";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "<<<->";
    for (int i = 0;i < n;++i) res += "[>+>+<<-]>[<+>-]>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += ">>>>>>-<<<<";
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>";
    res += "-]";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += ">>>>]<<<<<<<<]";
    for (int i = 0;i < n;++i) res += ">>[-]>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "\nend divide\n";
    return res;
}

string divideSigned(int n) {
    string res = "";
    res += "\nsigned divide\n";
    // ｂを右にシフト
    for (int i = 0;i < n;++i) res += ">>>>>>>>+";
    res += "[-<<<<<<[";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += "+";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "-]<<]";
    // ループ用フラグを立てる
    for (int i = 0;i < n;++i) res += ">>>>>>>>+";
    // 除算
    res += "[";
    for (int i = 0;i < n;++i) res += ">>[<<<<<<<<+>>>>>>>>-]>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "[<<<<<<<+>";
    for (int i = 0;i < n;++i) res += "[>+>+<<-]>[<+>-]>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += ">>";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+<<++++++++++>>>>>>>+<<<<<]<<-<]>>>>>>>>";
    res += "[-";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "<<<->";
    for (int i = 0;i < n;++i) res += "[>+>+<<-]>[<+>-]>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += ">>>>>>-<<<<";
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>";
    res += "-]";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += ">>>>]<<<<<<<<]";
    for (int i = 0;i < n;++i) res += ">>>>>>>>>>>>>>>>";
    res += ">>>>>[<+<<->>>-]<[>+<-]<<[+[-]<+>]<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<[-]<<";
    res += "\nend signed divide\n";
    return res;
}

// Unidenさんありがとうございます
string UnidenDivide(int n) {
    string res = "";
    res += "\ndivide Uniden\n";
    for (int i = 0;i < n;++i) res += ">>>>>>>+>";
    res += "<[<<<<<<<[>>>>>>>[-]>>>>>>>>[>>>>>>>>]";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "[->>>>>>>>]+[->>>>>>>>[>>>>>>>>]<<<<<<<+<[<<<<<<<<]>>>>>>>>]<<<<<<<-[";
    for (int i = 0;i < n;++i) res += "<<<<<<<<([->>>>>>>>+<<<<<<<<]";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">>+<<-]>>+[<+[<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+>>>>-<<<<<<<<<<<++++++++++>>]>+<<<-<]>>>>[-<<<<+>>>>]>>>>";
    res += "[->-<]>>>+>[+]<<<<>]<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[-> ---------[>>+<]>[<]>-[+<<---------->> >>>>>+>>>>-<<<<<<<<<]>+< << ++++++++++<] >>>>[-<<<<+>>>>]>>>>";
    res += "->>> >[+]<-[->>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "+";
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += "<<<<]<->>+<<[>]>[<<<<";
    for (int i = 0;i < n;++i) res += "<[->>>>>>>>+<<<<<<<<]<<<<<<<";
    for (int i = 0;i < n;++i) res += ">>>>>>>>[-<<<<<<<<+>>>>>>>>]";
    res += ">>>]>-<<]]<]<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "\nend divide Uniden\n";
    return res;
}

// v2をv1にコピー
// 0番地からスタート
string move(Attribute *v1,Attribute *v2,int start_unit1,int start_unit2,int index1,int index2,int len) {
    string res = "";
    res += "\nmove\n";
    res += movePointer(0,v2->address);
    res += movePointer(0,INTERVAL*start_unit2);
    for (int i = 0;i < len;++i) {
        res += "+";
        res += movePointer(0,INTERVAL);
    }
    res += movePointer(INTERVAL*len,0);
    res += "[-";
    res += movePointer(0,index2);
    res += "[-";
    res += movePointer(v2->address+INTERVAL*start_unit2+index2,v1->address+INTERVAL*start_unit1+index1);
    res += "+";
    res += movePointer(v1->address+INTERVAL*start_unit1+index1,v2->address+INTERVAL*start_unit2+index2);
    res += "]";
    res += movePointer(index2,INTERVAL);
    res += "]";
    res += movePointer(INTERVAL*len,0);
    res += movePointer(INTERVAL*start_unit2,0);
    res += movePointer(v2->address,0);
    res += "\nend move\n";
    return res;
}

string moveCharToInt(Attribute *v1,Attribute *v2,int index1,int index2) {
    string res = "";
    res += "\nmove char to int\n";
    res += movePointer(0,v2->address);
    res += movePointer(0,index2);

    res += "[";
    res += movePointer(v2->address+index2,v1->address+index1);
    res += "+";
    res += movePointer(v1->address+index1,v2->address+index2);
    res += "-]";

    int n = v1->idegit;
    res += movePointer(v2->address+index2,v1->address+index1);
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";

    res += movePointer(index1,0);
    res += movePointer(v1->address,0);
    res += "\nend move char to int\n";
    return res;
}

string moveIntToChar(Attribute *v1,Attribute *v2,int index1,int index2) {
    string res = "";
    res += "\nmove int to char\n";
    res += movePointer(0,v2->address);
    res += movePointer(0,INTERVAL*v2->fdegit);
    for (int i = 0;i < v2->idegit;++i) {
        res += "+";
        res += movePointer(0,INTERVAL);
    }
    res += movePointer(INTERVAL*v2->idegit,0);
    res += "[-";
    res += movePointer(0,index2);
    res += "[-";
    res += movePointer(v2->address+INTERVAL*v2->fdegit+index2,v1->address+index1);
    res += "+";
    res += movePointer(v1->address+index1,v2->address+INTERVAL*v2->fdegit+index2);
    res += "]";
    res += movePointer(index2,INTERVAL);
    res += "]";
    res += movePointer(v2->address+INTERVAL*(v2->idegit+v2->fdegit+1),v1->address+INTERVAL*v2->idegit+index1);
    for (int i = 0;i < v2->idegit-1;++i) {
        res += "[-";
        res += movePointer(INTERVAL,0);
        res += "++++++++++";
        res += movePointer(0,INTERVAL);
        res += "]";
        res += movePointer(INTERVAL,0);
    }
    res += movePointer(v1->address+index1,0);
    res += "\nend move int to char\n";
    return res;
}

string copy(Attribute *v1,Attribute *v2,int start_unit1,int start_unit2,int index1,int index2,int len,int empty_index) {
    string res = "";
    res += "\ncopy\n";
    res += movePointer(0,v2->address);
    res += movePointer(0,INTERVAL*start_unit2);
    for (int i = 0;i < len;++i) {
        res += "+";
        res += movePointer(0,INTERVAL);
    }
    res += movePointer(INTERVAL*len,0);
    res += "[-";
    res += movePointer(0,index2);
    res += "[-";
    res += movePointer(index2,empty_index);
    res += "+";
    res += movePointer(v2->address+INTERVAL*start_unit2+empty_index,v1->address+INTERVAL*start_unit1+index1);
    res += "+";
    res += movePointer(v1->address+INTERVAL*start_unit1+index1,v2->address+INTERVAL*start_unit2+index2);
    res += "]";
    res += movePointer(index2,empty_index);
    res += "[-";
    res += movePointer(empty_index,index2);
    res += "+";
    res += movePointer(index2,empty_index);
    res += "]";
    res += movePointer(empty_index,INTERVAL);
    res += "]";
    res += movePointer(INTERVAL*len,0);
    res += movePointer(INTERVAL*start_unit2,0);
    res += movePointer(v2->address,0);
    res += "\nend copy\n";
    return res;
}

string copyCharToInt(Attribute *v1,Attribute *v2,int index1,int index2,int empty_index) {
    string res = "";
    res += "\ncopy char to int\n";
    res += movePointer(0,v2->address);
    res += movePointer(0,index2);

    res += "[";

    res += movePointer(index2,empty_index);
    res += "+";
    res += movePointer(empty_index,index2);

    res += movePointer(v2->address+index2,v1->address+index1);
    res += "+";
    res += movePointer(v1->address+index1,v2->address+index2);
    res += "-]";

    res += movePointer(index2,empty_index);
    res += "[";
    res += movePointer(empty_index,index2);
    res += "+";
    res += movePointer(index2,empty_index);
    res += "-]";
    res += movePointer(empty_index,index2);

    int n = v1->idegit;
    res += movePointer(v2->address+index2,v1->address+index1);
    for (int i = 0;i < n;++i) res += "[->---------[>>+<]>[<]>-[+<<---------->>>>>>>+<<<<<]<<++++++++++<]>>>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";

    res += movePointer(index1,0);
    res += movePointer(v1->address,0);
    res += "\nend copy char to int\n";
    return res;
}

string copyIntToChar(Attribute *v1,Attribute *v2,int index1,int index2,int empty_index) {
    string res = "";
    res += "\ncopy int to char\n";
    res += movePointer(0,v2->address);
    res += movePointer(0,INTERVAL*v2->fdegit);
    for (int i = 0;i < v2->idegit;++i) {
        res += "+";
        res += movePointer(0,INTERVAL);
    }
    res += movePointer(INTERVAL*v2->idegit,0);
    res += "[-";
    res += movePointer(0,index2);
    res += "[-";
    res += movePointer(index2,empty_index);
    res += "+";
    res += movePointer(v2->address+INTERVAL*v2->fdegit+empty_index,v1->address+index1);
    res += "+";
    res += movePointer(v1->address+index1,v2->address+INTERVAL*v2->fdegit+index2);
    res += "]";
    res += movePointer(index2,empty_index);
    res += "[-";
    res += movePointer(empty_index,index2);
    res += "+";
    res += movePointer(index2,empty_index);
    res += "]";
    res += movePointer(empty_index,INTERVAL);
    res += "]";
    res += movePointer(v2->address+INTERVAL*(v2->idegit+v2->fdegit+1),v1->address+INTERVAL*v2->idegit+index1);
    for (int i = 0;i < v2->idegit-1;++i) {
        res += "[-";
        res += movePointer(INTERVAL,0);
        res += "++++++++++";
        res += movePointer(0,INTERVAL);
        res += "]";
        res += movePointer(INTERVAL,0);
    }
    res += movePointer(v1->address+index1,0);
    res += "\nend copy int to char\n";
    return res;
}

string clear(Attribute *v,int start_unit,int index,int len) {
    string res = "";
    res += "\nclear\n";
    res += movePointer(0,v->address);
    res += movePointer(0,start_unit*INTERVAL+index);
    for (int i = 0;i < len;++i) {
        res += "[-]";
        res += movePointer(0,INTERVAL);
    }
    res += movePointer(INTERVAL*(start_unit+len)+index,0);
    res += movePointer(v->address,0);
    res += "\nend clear\n";
    return res;
}

string printStr(int used_memory,string str) {
    string res = "";
    res += "\nprint str\n";
    res += movePointer(0,used_memory);
    for (int i = 0;i < str.size();++i) {
        for (int j = 0;j < str[i];++j) res += "+";
        res += ".";
        for (int j = 0;j < str[i];++j) res += "-";
    }
    res += movePointer(used_memory,0);
    res += "\nend print str\n";
    return res;
}

string printChar(Attribute *v,int index) {
    string res = "";
    res += "\nprint char\n";
    res += movePointer(0,v->address+index);
    res += ".";
    res += movePointer(v->address+index,0);
    res += "\nend print char\n";
    return res;
}

string printUint(Attribute *v,int index) {
    string res = "";
    res += "\nprint uint\n";
    res += movePointer(0,v->address+index);
    res += movePointer(0,INTERVAL*v->size());
    for (int i = 0;i < v->size()-1;++i) {
        res += movePointer(INTERVAL,0);
        res += "[>>+<]>[<]>[";
        res += movePointer(INTERVAL,0);
        res += "+";
        res += movePointer(0,INTERVAL-2);
        res += "++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------>>[-]]<<";
    }
    res += movePointer(INTERVAL-2,0);
    res += "[-]";
    res += "<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------";
    res += movePointer(v->address+index,0);
    res += "\nend print uint\n";
    return res;
}

string printInt(Attribute *v,int index) {
    string res = "";
    res += "\nprint int\n";
    res += movePointer(0,v->address+index);
    res += movePointer(0,INTERVAL*(v->size()-1));
    res += "[>>+<]>[<]>[++++++++++++++++++++++++++++++++++++++++++++.---------------------------------------------]<<";
    for (int i = 0;i < v->idegit-1;++i) {
        res += movePointer(INTERVAL,0);
        res += "[>>+<]>[<]>[";
        res += movePointer(INTERVAL,0);
        res += "+";
        res += movePointer(0,INTERVAL-2);
        res += "++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------>>[-]]<<";
    }
    res += movePointer(INTERVAL-2,0);
    res += "[-]";
    res += "<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------";
    res += movePointer(v->address+index,0);
    res += "\nend print int\n";
    return res;
}

string printFixed(Attribute *v,int index) {
    string res = "";
    res += "\nprint fixed\n";
    res += movePointer(0,v->address+index);
    res += movePointer(0,INTERVAL*(v->size()-1));
    res += "[>>+<]>[<]>[++++++++++++++++++++++++++++++++++++++++++++.---------------------------------------------]<<";
    for (int i = 0;i < v->idegit-1;++i) {
        res += movePointer(INTERVAL,0);
        res += "[>>+<]>[<]>[";
        res += movePointer(INTERVAL,0);
        res += "+";
        res += movePointer(0,INTERVAL-2);
        res += "++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------>>[-]]<<";
    }
    res += movePointer(INTERVAL-2,0);
    res += "[-]";
    res += "<<++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------";
    res += ">++++++++++++++++++++++++++++++++++++++++++++++.----------------------------------------------<";
    for (int i = 0;i < v->fdegit;++i) {
        res += movePointer(INTERVAL,0);
        res += "++++++++++++++++++++++++++++++++++++++++++++++++.------------------------------------------------";
    }
    res += movePointer(v->address+index,0);
    res += "\nend print fixed\n";
    return res;
}

string scanUint(Attribute *v,int index) {
    string res = "";
    res += "\nscan uint\n";
    res += movePointer(0,v->address+index);
    res += ">>+[-<,----------[----------------------[----------------<";
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(0,INTERVAL);
    for (int i = 0;i < v->idegit-1;++i) {
        res += movePointer(INTERVAL,0);
        res += "[";
        res += movePointer(0,INTERVAL);
        res += "+";
        res += movePointer(INTERVAL,0);
        res += "-]";
    }
    res += ">[<+>-]>+<]]>]<<";
    res += movePointer(v->address+index,0);
    res += "\nend scan uint\n";
    return res;
}

string scanInt(Attribute *v,int index) {
    string res = "";
    res += "\nscan int\n";
    res += movePointer(0,v->address+index);

    res += ",---------------------------------------------[--->>+<]>[<]>-[+";
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(0,INTERVAL);
    res += movePointer(0,INTERVAL-2);
    res += "+";
    res += movePointer(INTERVAL-2,0);
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(INTERVAL,0);

    res += "]+[-<,----------[----------------------[----------------<";
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(0,INTERVAL);
    for (int i = 0;i < v->idegit-1;++i) {
        res += movePointer(INTERVAL,0);
        res += "[";
        res += movePointer(0,INTERVAL);
        res += "+";
        res += movePointer(INTERVAL,0);
        res += "-]";
    }
    res += ">[<+>-]>+<]]>]<<";
    res += movePointer(v->address+index,0);
    res += "\nend scan int\n";
    return res;
}

string scanFixed(Attribute *v,int index) {
    string res = "";
    res += "\nscan fixed\n";
    res += movePointer(0,v->address+index);
    res += movePointer(0,INTERVAL*v->fdegit);

    res += ",---------------------------------------------[--->>+<]>[<]>-[+";
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(0,INTERVAL);
    res += movePointer(0,INTERVAL-2);
    res += "+";
    res += movePointer(INTERVAL-2,0);
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(INTERVAL,0);

    res += "]+[-<,----------------------------------------------[--<";
    for (int i = 0;i < v->idegit-1;++i) res += movePointer(0,INTERVAL);
    for (int i = 0;i < v->idegit-1;++i) {
        res += movePointer(INTERVAL,0);
        res += "[";
        res += movePointer(0,INTERVAL);
        res += "+";
        res += movePointer(INTERVAL,0);
        res += "-]";
    }
    res += ">[<+>-]>+<]>]+";
    for (int i = 0;i < v->fdegit-1;++i) {
        res += "[-<,----------[----------------------[----------------[<";
        res += movePointer(INTERVAL,0);
        res += "+";
        res += movePointer(0,INTERVAL);
        res += ">-]";
        res += movePointer(INTERVAL-1,0);
        res += "+";
        res += movePointer(0,INTERVAL-1);
        res += "]]>]";
        res += movePointer(INTERVAL,0);
    }
    res += "[-<,----------[----------------------[----------------[<";
    res += movePointer(INTERVAL,0);;
    res += "+";
    res += movePointer(0,INTERVAL);;
    res += ">-],----------[----------------------]]]>]<<";
    res += movePointer(INTERVAL,0);
    res += movePointer(v->address+index,0);
    res += "\nend scan fixed\n";
    return res;
}

string scanChar(Attribute *v,int index) {
    string res = "";
    res += "\nscan char\n";
    res += movePointer(0,v->address+index);
    res += ",";
    res += movePointer(v->address+index,0);
    res += "\nend scan char\n";
    return res;
}

string ifBegin(Attribute *v) {
    string res = "";
    res += "\nif\n";
    res += movePointer(0,v->address);
    res += ">[-<";
    res += movePointer(v->address,0);
    return res;
}

string ifEnd(Attribute *v) {
    string res = "";
    res += movePointer(0,v->address);
    res += ">]<";
    res += movePointer(v->address,0);
    res += "\nend if\n";
    return res;
}

string ifElseBegin(Attribute *v) {
    string res = "";
    res += "\nif\n";
    res += movePointer(0,v->address);
    res += ">>+<[->-<<";
    res += movePointer(v->address,0);
    return res;
}

string ifElseMid(Attribute *v) {
    string res = "";
    res += movePointer(0,v->address);
    res += ">]";
    res += "\nelse\n";
    res += ">[-<<";
    res += movePointer(v->address,0);

    return res;
}

string ifElseEnd(Attribute *v) {
    string res = "";
    res += movePointer(0,v->address);
    res += ">>]<<";
    res += movePointer(v->address,0);
    res += "\nend if\n";
    return res;
}

string whileBegin(Attribute *v) {
    string res = "";
    res += "\nwhile\n";
    res += movePointer(0,v->address);
    res += ">+[-<";
    res += movePointer(v->address,0);
    return res;
}

string whileMid(Attribute *v) {
    string res = "";
    res += "\nwhile mid\n";
    res += movePointer(0,v->address);
    res += ">>[-<<";
    res += movePointer(v->address,0);
    return res;
}

string whileEnd(Attribute *v) {
    string res = "";
    res += movePointer(0,v->address);
    res += ">>]<]<";
    res += movePointer(v->address,0);
    res += "\nend while\n";
    return res;
}

string equalUnsigned(Attribute *v) {
    string res = "";
    res += "\nequal unsigned\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>";
    res += ">>>+>[<->-]<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend equal unsigned\n";
    return res;
}

string notEqualUnsigned(Attribute *v) {
    string res = "";
    res += "\nnot equal unsigned\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">>[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend not equal unsigned\n";
    return res;
}

string lessUnsigned(Attribute *v) {
    string res = "";
    res += "\nless unsigned\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend less unsigned\n";
    return res;
}

string greaterEqualUnsigned(Attribute *v) {
    string res = "";
    res += "\ngreater equal unsigned\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>";
    res += ">>+<[>-<-]<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend greater equal unsigned\n";
    return res;
}

string equalSigned(Attribute *v) {
    string res = "";
    res += "\nequal signed\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">[>-<-]>>+<[>-<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>";
    res += ">-]>[<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>";
    res += ">>-]+>[<->-]<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend equal signed\n";
    return res;
}

string notEqualSigned(Attribute *v) {
    string res = "";
    res += "\nnot equal signed\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">>[>-<-]>>+<[>-<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>";
    res += ">-]>[<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->-<]>[+++++++++[-]>>+<]>[<]>[>>>>>>>>+<<<<<<<<[-]]>>>>>";
    res += ">>-]<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend not equal signed\n";
    return res;
}

string lessSigned(Attribute *v) {
    string res = "";
    res += "\nless signed\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">[>>+<<-]>[>++<-]>[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>";
    res += ">>>>]<<-[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[-]>[-]>>>>>>>";
    res += ">>>>]<<-[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>";
    res += ">>>[<<<+>>>-]>]<<-[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "+";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>";
    res += ">+<[>-<-]>[<+>-]>>>]<<[+]<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend less signed\n";
    return res;
}

string greaterEqualSigned(Attribute *v) {
    string res = "";
    res += "\ngreater equal signed\n";
    res += movePointer(0,v->address);
    int n = v->idegit+v->fdegit;
    for (int i = 0;i < n;++i) res += ">>>>>>>>";
    res += ">[>>+<<-]>[>++<-]>[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>";
    res += ">>>>]<<-[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[-]>[-]>>>>>>>";
    res += ">>>>]<<-[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    for (int i = 0;i < n;++i) res += "[[-]>>>+<<<]>[[-]>>+<<]>>[>>>>>>>>+<<<<<<<<[-]]>>>>>";
    res += ">>>[<<<+>>>-]>]<<-[>>+<]>[<]>-[+<<<<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += "+";
    for (int i = 0;i < n;++i) res += "[->[>>+<]>[<]>-[+>>>>>+<<<<<<<++++++++++>>]<<-<]>[-]>>>>>>>";
    res += ">+<[>-<-]>[<+>-]>>>]<<[+]<+<[>-<-]<";
    for (int i = 0;i < n;++i) res += "<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend greater equal signed\n";
    return res;
}

string equalChar(Attribute *v) {
    string res = "";
    res += "\nequal char\n";
    res += movePointer(0,v->address);
    res += ">[>-<-]>[>>>>>>>>>>+<<<<<<<<<<[-]]>>>>>>>>>+>[<->-]<<<<<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend equal char\n";
    return res;
}

string notEqualChar(Attribute *v) {
    string res = "";
    res += "\nnot equal char\n";
    res += movePointer(0,v->address);
    res += ">>[>-<-]>[>>>>>>>>>>+<<<<<<<<<<[-]]<<<";
    res += movePointer(v->address,0);
    res += "\nend not equal char\n";
    return res;
}

string lessChar(Attribute *v) {
    string res = "";
    res += "\nless char\n";
    res += movePointer(0,v->address);
    res += ">[->[>>+<]>[<]>-[+>>>>>+<<<<<<<<[-]>+>>]<<-<]>[-]<<";
    res += movePointer(v->address,0);
    res += "\nend less char\n";
    return res;
}

string greaterEqualChar(Attribute *v) {
    string res = "";
    res += "\ngreater equal char\n";
    res += movePointer(0,v->address);
    res += ">[->[>>+<]>[<]>-[+>>>>>+<<<<<<<<[-]>+>>]<<-<]>[-]>>>>>>>>+<[>-<-]<<<<<<<<<";
    res += movePointer(v->address,0);
    res += "\nend greater equal char\n";
    return res;
}

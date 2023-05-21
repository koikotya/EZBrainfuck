#ifndef INCLUDED_BF
#define INCLUDED_BF

#include "ast.h"

void movePointer(int p1,int p2) ;
void setSign(Variable* v,int index,bool sign) ;
void turnSign(Variable* v,int index,bool sign) ;
void setChar(Variable* v,int index,char *literal) ;
void setInteger(Variable* v,int index,char *literal) ;
void setDecimal(Variable* v,int index,char *literal,Operation op) ;
void add(int n) ;
void sub(int n) ;
void addSigned(int n) ;
void subSigned(int n) ;
void addChar() ;
void subChar() ;
void mult(int n) ;
void multSigned(int n) ;
void multShort(int n) ;
void multSignedShort(int n) ;
void divide(int n) ;
void divideSigned(int n) ;
void UnidenDivide(int n) ;
void move(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len) ;
void moveCharToInt(Variable *v1,Variable *v2,int index1,int index2);
void moveIntToChar(Variable *v1,Variable *v2,int index1,int index2);
void copy(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len,int empty_index) ;
void copyCharToInt(Variable *v1,Variable *v2,int index1,int index2,int empty_index);
void copyIntToChar(Variable *v1,Variable *v2,int index1,int index2,int empty_index);
void clear(Variable *v,int start_unit,int index,int len) ;
void printStr(int used_memory,char *str) ;
void printChar(Variable *v,int index) ;
void printUint(Variable *v,int index) ;
void printInt(Variable *v,int index) ;
void printFixed(Variable *v,int index) ;
void scanUint(Variable *v,int index) ;
void scanInt(Variable *v,int index) ;
void scanFixed(Variable *v,int index) ;
void scanChar(Variable *v,int index) ;
void ifBegin(Variable *v) ;
void ifEnd(Variable *v) ;
void ifElseBegin(Variable *v) ;
void ifElseMid(Variable *v) ;
void ifElseEnd(Variable *v) ;
void whileBegin(Variable *v) ;
void whileMid(Variable *v) ;
void whileEnd(Variable *v) ;
void equalUnsigned(Variable *v) ;
void notEqualUnsigned(Variable *v) ;
void lessUnsigned(Variable *v) ;
void greaterEqualUnsigned(Variable *v) ;
void equalSigned(Variable *v) ;
void notEqualSigned(Variable *v) ;
void lessSigned(Variable *v) ;
void greaterEqualSigned(Variable *v) ;
void equalChar(Variable *v) ;
void notEqualChar(Variable *v) ;
void lessChar(Variable *v) ;
void greaterEqualChar(Variable *v) ;

#endif
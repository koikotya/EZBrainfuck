#ifndef INCLUDED_BF
#define INCLUDED_BF

#include <string>

#include "attribute.hpp"

using std::string;

string setLiteral(Attribute *v1,Attribute *v2,int index1) ;
string setValue(Attribute *v1,Attribute *v2,int index1) ;
string movePointer(int p1,int p2) ;
string setSign(Attribute* v,int index,bool sign) ;
string turnSign(Attribute* v,int index,bool sign) ;
string setChar(Attribute* v,int index,string literal) ;
string setInteger(Attribute* v,int index,string literal) ;
string setDecimal(Attribute* v1,int index,Attribute* v2) ;
string add(int n) ;
string sub(int n) ;
string addSigned(int n) ;
string subSigned(int n) ;
string addChar() ;
string subChar() ;
string mult(int n) ;
string multSigned(int n) ;
string multShort(int n) ;
string multSignedShort(int n) ;
string divide(int n) ;
string divideSigned(int n) ;
string UnidenDivide(int n) ;
string move(Attribute *v1,Attribute *v2,int start_unit1,int start_unit2,int index1,int index2,int len) ;
string moveCharToInt(Attribute *v1,Attribute *v2,int index1,int index2);
string moveIntToChar(Attribute *v1,Attribute *v2,int index1,int index2);
string copy(Attribute *v1,Attribute *v2,int start_unit1,int start_unit2,int index1,int index2,int len,int empty_index) ;
string copyCharToInt(Attribute *v1,Attribute *v2,int index1,int index2,int empty_index);
string copyIntToChar(Attribute *v1,Attribute *v2,int index1,int index2,int empty_index);
string clear(Attribute *v,int start_unit,int index,int len) ;
string printStr(int used_memory,string str) ;
string printChar(Attribute *v,int index) ;
string printUint(Attribute *v,int index) ;
string printInt(Attribute *v,int index) ;
string printFixed(Attribute *v,int index) ;
string scanUint(Attribute *v,int index) ;
string scanInt(Attribute *v,int index) ;
string scanFixed(Attribute *v,int index) ;
string scanChar(Attribute *v,int index) ;
string ifBegin(Attribute *v) ;
string ifEnd(Attribute *v) ;
string ifElseBegin(Attribute *v) ;
string ifElseMid(Attribute *v) ;
string ifElseEnd(Attribute *v) ;
string whileBegin(Attribute *v) ;
string whileMid(Attribute *v) ;
string whileEnd(Attribute *v) ;
string equalUnsigned(Attribute *v) ;
string notEqualUnsigned(Attribute *v) ;
string lessUnsigned(Attribute *v) ;
string greaterEqualUnsigned(Attribute *v) ;
string equalSigned(Attribute *v) ;
string notEqualSigned(Attribute *v) ;
string lessSigned(Attribute *v) ;
string greaterEqualSigned(Attribute *v) ;
string equalChar(Attribute *v) ;
string notEqualChar(Attribute *v) ;
string lessChar(Attribute *v) ;
string greaterEqualChar(Attribute *v) ;

#endif

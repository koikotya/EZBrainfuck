#ifndef INCLUDED_ATTRIBUTE
#define INCLUDED_ATTRIBUTE

#include <string>

using std::string;

enum Type {
    BOOL_TYPE = 2000,
    CHAR_TYPE,
    UINT_TYPE,
    INT_TYPE,
    FIXED_TYPE,
};

enum Operation {
    INT_LITERAL = 3000,
    DECIMAL_LITERAL,
    PLUS_OP, // 計算中の値
    MINUS_OP,
    TIMES_OP,
    DIVIDE_OP,
    MOD_OP,
    EQUAL_COND,
    NOTEQUAL_COND,
    LESS_COND,
    GREATEREQUAL_COND,
    NOT_OP,
};

struct Attribute {
    Type type;
    Operation op;
    int idegit; // 整数部の桁数
    int fdegit; // 小数部の桁数
    bool sign; // 符号の有無（int,fixed、その他ifやwhile等でも使用）
    int address; // メモリ使用開始位置（左端の位置）
    bool negative; // int,fixedの時のみ使用、符号を管理
    string ident; // 変数名、リテラルを保存

    Attribute() {};
};

#endif
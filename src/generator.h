// このヘッダファイルは使わない

#include <stdbool.h>

// dfsの引数のとき使う
typedef enum {
    UINT_TYPE,
    INT_TYPE,
    FIXED_TYPE,
    CHAR_TYPE,
    BOOL_TYPE,
} Type;
// dfsの戻り値のとき使う
typedef enum {
    INT_LITERAL,
    DECIMAL_LITERAL,
    PLUS_OP, // 計算中の値
    MINUS_OP,
    TIMES_OP,
    DIVIDE_OP,
    MOD_OP,
    NOT_OP,
} Operation;

typedef struct{
    Type type;
    Operation op;
    int idegit; // 整数部の桁数
    int fdegit; // 小数部の桁数
    bool sign; // int,fixedでは常にtrue、それ以外では常にfalse
    // int size; // ユニットの数を表す。idegit+fdegit+signに等しい
    int location; // メモリ使用開始位置（左端の位置）
    int unit_size; // 変数では2か3、計算中の値では8
    bool negative; // int,fixedの時のみ使用、符号を管理
    char *ident; // 変数名、リテラルを保存
} Variable;

int size(Variable *v) ;

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
void shortMult(int n) ;
void shortMultSigned(int n) ;
void divide(int n) ;
void divideSigned(int n) ;
void UnidenDivide(int n) ;
void move(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len) ;
void copy(Variable *v1,Variable *v2,int start_unit1,int start_unit2,int index1,int index2,int len,int empty_index) ;
void clear(Variable *v,int start_unit,int index,int len) ;
void printChar(Variable *v) ;
void printUint(Variable *v) ;
void printInt(Variable *v) ;
void printFixed(Variable *v) ;
void scanUint(Variable *v) ;
void scanInt(Variable *v) ;
void scanFixed(Variable *v) ;
void scanChar(Variable *v) ;
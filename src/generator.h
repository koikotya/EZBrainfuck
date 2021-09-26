#include <stdbool.h>

// dfsの引数のとき使う
typedef enum {
    UINT,
    INT,
    FIXED,
    CHAR,
    BOOL,
} Typename;
// dfsの戻り値のとき使う
typedef enum {
    INT_LITERAL,
    DECIMAL_LITERAL,
    PLUS_TEMP, // 計算中の値
    MINUS_TEMP,
    TIMES_TEMP,
    DIVIDE_TEMP,
    MOD_TEMP,
    NOT_OP,
} Operation;

typedef struct{
    Typename type;
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
void signedAdd(int n) ;
void signedSub(int n) ;
void charAdd() ;
void charSub() ;
void mult(int n) ;
void signedMult(int n) ;
void multShort(int n) ;
void signedMultShort(int n) ;
void divide(int n) ;
void signedDivide(int n) ;
void divide_Uniden(int n) ;
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
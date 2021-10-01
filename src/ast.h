#include <stdbool.h>

typedef enum {
    PLUS_AST = 0,
    MINUS_AST,
    TIMES_AST,
    DIVIDE_AST,
    MOD_AST,
    ASSIGN_AST,
    INTNUMBER_AST,
    DECIMALNUMBER_AST,
    IF_AST,
    IF_ELSE_AST,
    WHILE_AST,
    UINT_AST,
    INT_AST,
    FIXED_AST,
    BOOL_AST,
    CHAR_AST,
    STR_AST,
    EQUAL_AST,
    NOTEQUAL_AST,
    LESS_AST,
    LESSEQUAL_AST,
    GREATER_AST,
    GREATEREQUAL_AST,
    IDENT_AST,
    SCAN_AST,
    PRINT_AST,
    MAIN_AST,
    STATEMENTS_AST,
} Ntype;

// dfsの引数のとき使う
typedef enum {
    UINT_TYPE = 2000,
    INT_TYPE,
    FIXED_TYPE,
    CHAR_TYPE,
    BOOL_TYPE,
} Typename;
// dfsの戻り値のとき使う
typedef enum {
    INT_LITERAL = 3000,
    DECIMAL_LITERAL,
    PLUS_TEMP, // 計算中の値
    MINUS_TEMP,
    TIMES_TEMP,
    DIVIDE_TEMP,
    MOD_TEMP,
    EQUAL_COND,
    NOTEQUAL_COND,
    LESS_COND,
    LESSEQUAL_COND,
    GREATER_COND,
    GREATEREQUAL_COND,
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

typedef struct node {
    Ntype type;
    int n;
    struct node **list;
    char str[256];
    Variable *v;
} Node;

Node* build_str_node(Ntype t,char s[]);
Node* build_node1(Ntype t,Node* p1);
Node* build_node2(Ntype t,Node* p1,Node* p2);
Node* build_node3(Ntype t,Node* p1,Node* p2,Node* p3);
void print_node(Node *p,int i);

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
void printStr(int used_memory,char *str) ;
void printChar(Variable *v) ;
void printUint(Variable *v) ;
void printInt(Variable *v) ;
void printFixed(Variable *v) ;
void scanUint(Variable *v) ;
void scanInt(Variable *v) ;
void scanFixed(Variable *v) ;
void scanChar(Variable *v) ;
void ifBegin(Variable *v) ;
void ifEnd(Variable *v) ;
void ifElseBegin(Variable *v) ;
void ifElseMid(Variable *v) ;
void ifElseEnd(Variable *v) ;
void equalUnsigned(Variable *v) ;
void lessUnsigned(Variable *v) ;
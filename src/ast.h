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
    GREATEREQUAL_AST,
    IDENT_AST,
    SCAN_AST,
    PRINT_AST,
    MAIN_AST,
    STATEMENTS_AST,
} Ntype;

typedef enum {
    BOOL_TYPE = 2000,
    CHAR_TYPE,
    UINT_TYPE,
    INT_TYPE,
    FIXED_TYPE,
} Type;

typedef enum {
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
} Operation;

typedef struct{
    Type type;
    Operation op;
    int idegit; // 整数部の桁数
    int fdegit; // 小数部の桁数
    bool sign; // int,fixedでは常にtrue、それ以外では常にfalse
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
void print_err(char *s);

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

// emscriptenでのコンパイルエラー回避に必要
int yylex(void);
int yyerror(char const *str);
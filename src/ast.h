#ifndef INCLUDED_AST
#define INCLUDED_AST

#include <stdbool.h>

#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) < (b)) ? (b) : (a))

#define INTERVAL 8

typedef enum {
    UNARY_PLUS_AST = 0,
    UNARY_MINUS_AST,
    PLUS_AST,
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
    int address; // メモリ使用開始位置（左端の位置）
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

Node* buildStrNode(Ntype t,char s[]);
Node* buildNode1(Ntype t,Node* p1);
Node* buildNode2(Ntype t,Node* p1,Node* p2);
Node* buildNode3(Ntype t,Node* p1,Node* p2,Node* p3);
void printNode(Node *p,int i);

// emscriptenでのコンパイルエラー回避に必要
int yylex(void);
int yyerror(char const *str);

#endif
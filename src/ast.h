typedef enum {
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
    LESSEQUAL_AST,
    GREATER_AST,
    GREATEREQUAL_AST,
    IDENT_AST,
    SCAN_AST,
    PRINT_AST,
    MAIN_AST,
    STATEMENTS_AST,
} Ntype;

typedef struct node {
    Ntype type;
    int n;
    struct node **list;
    char str[256];
} Node;

Node* build_str_node(Ntype t,char s[]);
Node* build_node1(Ntype t,Node* p1);
Node* build_node2(Ntype t,Node* p1,Node* p2);
Node* build_node3(Ntype t,Node* p1,Node* p2,Node* p3);
void print_node(Node *p,int i);
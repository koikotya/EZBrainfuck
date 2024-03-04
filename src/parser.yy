%require "3.2"
%language "c++"
%header "parser.hpp"

%define api.value.type variant
%define api.token.constructor

%code requires {
    class Driver;
    struct Node;
}

%param {Driver& drv}

%define parse.trace
%define parse.error detailed

%code {
#include "node.hpp"
#include "driver.hpp"
}

%%

%token FN MAIN;
%token LPAREN RPAREN LBRACE RBRACE;
%token COMMA SEMI;
%token PLUS MINUS TIMES DIVIDE MOD;
%token EQUAL NOTEQUAL LESS LESSEQUAL GREATER GREATEREQUAL;
%token ASSIGN;
%token IF ELSE WHILE;
%token UINT INT FIXED BOOL CHAR;
%token SCAN PRINT;

%token<Node*> INTNUMBER DECIMALNUMBER STR IDENT;

%nterm<Node*> statements statement if_statement io condition declaration assignment;
%nterm<Node*> expression term factor primary_expression;

program :
    FN MAIN LPAREN RPAREN LBRACE statements RBRACE {
        drv.root_ = new Node(MAIN_AST,$6);
    }
    ;

statements :
    {
        $$ = nullptr;
    }
    | statement statements {
        $$ = new Node(STATEMENTS_AST,$1,$2);
    }
    ;

statement :
    declaration SEMI {
        $$ = $1;
    }
    | assignment SEMI {
        $$ = $1;
    }
    | io SEMI {
        $$ = $1;
    }
    | if_statement {
        $$ = $1;
    }
    | WHILE LPAREN condition RPAREN LBRACE statements RBRACE {
        $$ = new Node(WHILE_AST,$3,$6);
    }
    ;

if_statement : 
    IF LPAREN condition RPAREN LBRACE statements RBRACE {
        $$ = new Node(IF_STATEMENT_AST,$3,new Node(IF_ELSE_AST,$6));
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE {
        $$ = new Node(IF_STATEMENT_AST,$3,new Node(IF_ELSE_AST,$6,$10));
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE ELSE if_statement {
        $$ = new Node(IF_STATEMENT_AST,$3,new Node(IF_ELSE_AST,$6,$9));
    }
    ;

io
    : SCAN LPAREN IDENT RPAREN {
        $$ = new Node(SCAN_AST,$3);
    }
    | PRINT LPAREN expression RPAREN {
        $$ = new Node(PRINT_AST,$3);
    }
    | PRINT LPAREN STR RPAREN {
        $$ = new Node(PRINT_AST,$3);
    }
    ;

condition
    : expression EQUAL expression {
        $$ = new Node(EQUAL_AST,$1,$3);
    }
    | expression NOTEQUAL expression {
        $$ = new Node(NOTEQUAL_AST,$1,$3);
    }
    | expression LESS expression {
        $$ = new Node(LESS_AST,$1,$3);
    }
    | expression LESSEQUAL expression {
        $$ = new Node(GREATEREQUAL_AST,$3,$1);
    }
    | expression GREATER expression {
        $$ = new Node(LESS_AST,$3,$1);
    }
    | expression GREATEREQUAL expression {
        $$ = new Node(GREATEREQUAL_AST,$1,$3);
    }
    ;

declaration :
    UINT LPAREN INTNUMBER RPAREN IDENT {
        $$ = new Node(UINT_AST,new Node(DIGITS_LIST_AST,$3),$5);
    }
    | INT LPAREN INTNUMBER RPAREN IDENT {
        $$ = new Node(INT_AST,new Node(DIGITS_LIST_AST,$3),$5);
    }
    | FIXED LPAREN INTNUMBER COMMA INTNUMBER RPAREN IDENT {
        $$ = new Node(FIXED_AST,new Node(DIGITS_LIST_AST,$3,$5),$7);
    } 
    | BOOL IDENT {
        $$ = new Node(BOOL_AST,$2);
    }
    | CHAR IDENT {
        $$ = new Node(CHAR_AST,$2);
    }
    ;

assignment
    : IDENT ASSIGN expression {
        $$ = new Node(ASSIGN_AST,$1,$3);
    }
    ;

expression
    : term {
        $$ = $1;
    }
    | expression PLUS term {
        $$ = new Node(PLUS_AST,$1,$3);
    }
    | expression MINUS term {
        $$ = new Node(MINUS_AST,$1,$3);
    }
    ;


term
    : factor {
        $$ = $1;
    }
    | term TIMES factor {
        $$ = new Node(TIMES_AST,$1,$3);
    }
    | term DIVIDE factor {
        $$ = new Node(DIVIDE_AST,$1,$3);
    }
    | term MOD factor {
        $$ = new Node(MOD_AST,$1,$3);
    }
    ;

factor
    : primary_expression {
        $$ = $1;
    }
    | PLUS primary_expression {
        $$ = new Node(UNARY_PLUS_AST,$2);
    }
    | MINUS primary_expression {
        $$ = new Node(UNARY_MINUS_AST,$2);
    }
    ;

primary_expression
    : IDENT {
        $$ = $1;
    }
    | INTNUMBER {
        $$ = $1;
    }
    | DECIMALNUMBER {
        $$ = $1;
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;
%%

namespace yy {
    auto parser::error(const std::string& msg) -> void {
        std::cerr << msg << "\n";
    }
}
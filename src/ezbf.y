%{
#include <stdio.h>
#include <stdlib.h>
// #define YYDEBUG 1
#include "ast.h"
#include "func.h"

Node *parse_result = NULL;

%}

%union {
    Node *p;
}

%token<p> PLUS MINUS TIMES DIVIDE MOD
%token<p> ASSIGN
%token<p> IF ELSE WHILE
%token<p> UINT INT FIXED BOOL CHAR 
%token<p> EQUAL NOTEQUAL LESS LESSEQUAL GREATER GREATEREQUAL
%token<p> SCAN PRINT
%token<p> INTNUMBER DECIMALNUMBER STR IDENT
%token FN MAIN
%token LPAREN RPAREN LBRACE RBRACE
%token COMMA SEMI

%type<p> statements statement io condition declaration assignment expression term factor primary_expression
%%

program
    : FN MAIN LPAREN RPAREN LBRACE statements RBRACE {
        parse_result = (Node *)malloc(sizeof(Node));
        parse_result = buildNode1(MAIN_AST,$6);
    }
    ;

statements
    : {
        $$ = NULL
    }
    | statement statements {
        $$ = buildNode2(STATEMENTS_AST,$1,$2);
    }
    ;

statement
    : declaration SEMI {
        $$ = $1;
    }
    | assignment SEMI {
        $$ = $1;
    }
    | io SEMI {
        $$ = $1;
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE {
        $$ = buildNode2(IF_AST,$3,$6);
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE {
        $$ = buildNode3(IF_ELSE_AST,$3,$6,$10);
    }
    | WHILE LPAREN condition RPAREN LBRACE statements RBRACE {
        $$ = buildNode2(WHILE_AST,$3,$6);
    }
    ;

io
    : SCAN LPAREN IDENT RPAREN {
        $$ = buildNode1(SCAN_AST,$3);
    }
    | PRINT LPAREN expression RPAREN {
        $$ = buildNode1(PRINT_AST,$3);
    }
    | PRINT LPAREN STR RPAREN {
        $$ = buildNode1(PRINT_AST,$3);
    }
    ;

condition
    : expression EQUAL expression {
        $$ = buildNode2(EQUAL_AST,$1,$3);
    }
    | expression NOTEQUAL expression {
        $$ = buildNode2(NOTEQUAL_AST,$1,$3);
    }
    | expression LESS expression {
        $$ = buildNode2(LESS_AST,$1,$3);
    }
    | expression LESSEQUAL expression {
        $$ = buildNode2(GREATEREQUAL_AST,$3,$1);
    }
    | expression GREATER expression {
        $$ = buildNode2(LESS_AST,$3,$1);
    }
    | expression GREATEREQUAL expression {
        $$ = buildNode2(GREATEREQUAL_AST,$1,$3);
    }
    ;

declaration
    : UINT LPAREN INTNUMBER RPAREN IDENT {
        $$ = buildNode2(UINT_AST,$3,$5);
    }
    | INT LPAREN INTNUMBER RPAREN IDENT {
        $$ = buildNode2(INT_AST,$3,$5);
    }
    | FIXED LPAREN INTNUMBER COMMA INTNUMBER RPAREN IDENT {
        $$ = buildNode3(FIXED_AST,$3,$5,$7);
    }
    | BOOL IDENT {
        $$ = buildNode1(BOOL_AST,$2);
    }
    | CHAR IDENT {
        $$ = buildNode1(CHAR_AST,$2);
    }
    ;

assignment
    : IDENT ASSIGN expression {
        $$ = buildNode2(ASSIGN_AST,$1,$3);
    }
    ;

expression
    : term {
        $$ = $1;
    }
    | expression PLUS term {
        $$ = buildNode2(PLUS_AST,$1,$3);
    }
    | expression MINUS term {
        $$ = buildNode2(MINUS_AST,$1,$3);
    }
    ;

term
    : factor {
        $$ = $1;
    }
    | term TIMES factor {
        $$ = buildNode2(TIMES_AST,$1,$3);
    }
    | term DIVIDE factor {
        $$ = buildNode2(DIVIDE_AST,$1,$3);
    }
    | term MOD factor {
        $$ = buildNode2(MOD_AST,$1,$3);
    }
    ;

factor
    : primary_expression {
        $$ = $1;
    }
    | PLUS primary_expression {
        $$ = buildNode1(UNARY_PLUS_AST,$2);
    }
    | MINUS primary_expression {
        $$ = buildNode1(UNARY_MINUS_AST,$2);
    }
    ;

primary_expression
    : IDENT {
        $$ = $1
    }
    | INTNUMBER {
        $$ = $1
    }
    | DECIMALNUMBER {
        $$ = $1
    }
    | LPAREN expression RPAREN {
        $$ = $2;
    }
    ;

%%

int yyerror(char const *str) {
    extern char *yytext;
    printErr("parser error near ");
    printErr(yytext);
    printErr("\n");
    return 0;
}
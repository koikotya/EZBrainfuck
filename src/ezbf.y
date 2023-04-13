%{
#include <stdio.h>
#include <stdlib.h>
// #define YYDEBUG 1
#include "ast.h"

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
        parse_result = build_node1(MAIN_AST,$6);
    }
    ;

statements
    : {
        $$ = NULL
    }
    | statement statements {
        $$ = build_node2(STATEMENTS_AST,$1,$2);
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
        $$ = build_node2(IF_AST,$3,$6);
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE {
        $$ = build_node3(IF_ELSE_AST,$3,$6,$10);
    }
    | WHILE LPAREN condition RPAREN LBRACE statements RBRACE {
        $$ = build_node2(WHILE_AST,$3,$6);
    }
    ;

io
    : SCAN LPAREN IDENT RPAREN {
        $$ = build_node1(SCAN_AST,$3);
    }
    | PRINT LPAREN IDENT RPAREN {
        $$ = build_node1(PRINT_AST,$3);
    }
    | PRINT LPAREN STR RPAREN {
        $$ = build_node1(PRINT_AST,$3);
    }
    ;

condition
    : expression EQUAL expression {
        $$ = build_node2(EQUAL_AST,$1,$3);
    }
    | expression NOTEQUAL expression {
        $$ = build_node2(NOTEQUAL_AST,$1,$3);
    }
    | expression LESS expression {
        $$ = build_node2(LESS_AST,$1,$3);
    }
    | expression LESSEQUAL expression {
        $$ = build_node2(GREATEREQUAL_AST,$3,$1);
    }
    | expression GREATER expression {
        $$ = build_node2(LESS_AST,$3,$1);
    }
    | expression GREATEREQUAL expression {
        $$ = build_node2(GREATEREQUAL_AST,$1,$3);
    }
    ;

declaration
    : UINT LPAREN INTNUMBER RPAREN IDENT {
        $$ = build_node2(UINT_AST,$3,$5);
    }
    | INT LPAREN INTNUMBER RPAREN IDENT {
        $$ = build_node2(INT_AST,$3,$5);
    }
    | FIXED LPAREN INTNUMBER COMMA INTNUMBER RPAREN IDENT {
        $$ = build_node3(FIXED_AST,$3,$5,$7);
    }
    | BOOL IDENT {
        $$ = build_node1(BOOL_AST,$2);
    }
    | CHAR IDENT {
        $$ = build_node1(CHAR_AST,$2);
    }
    ;

assignment
    : IDENT ASSIGN expression {
        $$ = build_node2(ASSIGN_AST,$1,$3);
    }
    ;

expression
    : term {
        $$ = $1;
    }
    | expression PLUS term {
        $$ = build_node2(PLUS_AST,$1,$3);
    }
    | expression MINUS term {
        $$ = build_node2(MINUS_AST,$1,$3);
    }
    ;

term
    : factor {
        $$ = $1;
    }
    | term TIMES factor {
        $$ = build_node2(TIMES_AST,$1,$3);
    }
    | term DIVIDE factor {
        $$ = build_node2(DIVIDE_AST,$1,$3);
    }
    | term MOD factor {
        $$ = build_node2(MOD_AST,$1,$3);
    }
    ;

factor
    : primary_expression {
        $$ = $1;
    }
    | PLUS primary_expression {
        $$ = build_node1(PLUS_AST,$2);
    }
    | MINUS primary_expression {
        $$ = build_node1(MINUS_AST,$2);
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
    print_err("parser error near ");
    print_err(yytext);
    print_err("\n");
    return 0;
}
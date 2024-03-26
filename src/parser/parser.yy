%require "3.2"
%language "c++"
%header "parser.hpp"

%define api.value.type variant
%define api.token.constructor

%code requires {
    class Driver;
    class Node;
}

%param {Driver& drv}

%define parse.trace
%define parse.error detailed

%code {
#include "all_node.hpp"
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

%token<char*> STR;
%token<Node*> INTNUMBER DECIMALNUMBER IDENT;

%nterm<Node*> statements statement if_statement io condition declaration assignment;
%nterm<Node*> expression term factor primary_expression;

program :
    FN MAIN LPAREN RPAREN LBRACE statements RBRACE {
        drv.root_ = new Root({$6});
    }
    ;

statements :
    {
        $$ = new Statements({});
    }
    | statements statement {
        $1->childs_.push_back($2);
        $$ = $1;
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
        $$ = new While({$3,$6});
    }
    ;

if_statement : 
    IF LPAREN condition RPAREN LBRACE statements RBRACE {
        $$ = new If({$3,$6});
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE ELSE LBRACE statements RBRACE {
        $$ = new IfElse({$3,$6,$10});
    }
    | IF LPAREN condition RPAREN LBRACE statements RBRACE ELSE if_statement {
        $$ = new IfElse({$3,$6,$9});
    }
    ;

io
    : SCAN LPAREN IDENT RPAREN {
        $$ = new Scan({$3});
    }
    | PRINT LPAREN expression RPAREN {
        if ($3->attr_.op == INT_LITERAL || $3->attr_.op == DECIMAL_LITERAL) {
            $$ = new PrintLiteral({$3});
        } else {
            $$ = new PrintNode({$3});
        }
    }
    | PRINT LPAREN STR RPAREN {
        $$ = new PrintStr($3);
    }
    ;

condition
    : expression EQUAL expression {
        $$ = new Equal({$1,$3});
    }
    | expression NOTEQUAL expression {
        $$ = new NotEqual({$1,$3});
    }
    | expression LESS expression {
        $$ = new Less({$1,$3});
    }
    | expression LESSEQUAL expression {
        $$ = new GreaterEqual({$3,$1});
    }
    | expression GREATER expression {
        $$ = new Less({$3,$1});
    }
    | expression GREATEREQUAL expression {
        $$ = new GreaterEqual({$1,$3});
    }
    ;

declaration :
    UINT LPAREN INTNUMBER RPAREN IDENT {
        $$ = new DeclUint({$5,$3});
    }
    | INT LPAREN INTNUMBER RPAREN IDENT {
        $$ = new DeclInt({$5,$3});
    }
    | FIXED LPAREN INTNUMBER COMMA INTNUMBER RPAREN IDENT {
        $$ = new DeclFixed({$7,$3,$5});
    } 
    | CHAR IDENT {
        $$ = new DeclChar({$2});
    }
    | BOOL IDENT {
        $$ = new DeclBool({$2});
    }
    ;

assignment
    : IDENT ASSIGN expression {
        $$ = new Assign({$1,$3});
    }
    ;

expression
    : term {
        $$ = $1;
    }
    | expression PLUS term {
        $$ = new Plus({$1,$3});
    }
    | expression MINUS term {
        $$ = new Minus({$1,$3});
    }
    ;


term
    : factor {
        $$ = $1;
    }
    | term TIMES factor {
        $$ = new Times({$1,$3});
    }
    | term DIVIDE factor {
        $$ = new Divide({$1,$3});
    }
    | term MOD factor {
        $$ = new Mod({$1,$3});
    }
    ;

factor
    : primary_expression {
        $$ = $1;
    }
    | PLUS primary_expression {
        $$ = new UnaryPlus({$2});
    }
    | MINUS primary_expression {
        $$ = new UnaryMinus({$2});
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

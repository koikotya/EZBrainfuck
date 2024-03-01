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

%token UINT;
%token<Node*> IDENT;
%token<Node*> INTNUMBER;

%nterm<Node*> statements;
%nterm<Node*> statement;
%nterm<Node*> declaration;

program:
    FN MAIN LPAREN RPAREN LBRACE statements RBRACE {
        Node* node = new Node(MAIN_AST,$6);
        drv.root_ = node;
    }
    ;

statements:
    {
        $$ = nullptr;
    }
    | statement statements {
        Node* node = new Node(STATEMENTS_AST,$1,$2);
        $$ = node;
    }
    ;

statement:
    declaration SEMI {
        $$ = $1;
    }
    ;

declaration:
    UINT LPAREN INTNUMBER RPAREN IDENT {
        Node* node = new Node(UINT_AST,$3,$5);
        $$ = node;
    }
    ;
%%

namespace yy {
    auto parser::error(const std::string& msg) -> void {
        std::cerr << msg << "\n";
    }
}
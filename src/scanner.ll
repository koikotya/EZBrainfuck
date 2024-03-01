%{
#include "node.hpp"
#include "driver.hpp"
#include "parser.hpp"

%}

%option noyywrap
%option header-file="scanner.hpp"

%{
%}

%%
"("                    {return yy::parser::make_LPAREN();}
")"                    {return yy::parser::make_RPAREN();}
"{"                    {return yy::parser::make_LBRACE();}
"}"                    {return yy::parser::make_RBRACE();}
","                    {return yy::parser::make_COMMA();}
";"                    {return yy::parser::make_SEMI();}
"fn"                   {return yy::parser::make_FN();}
"main"                 {return yy::parser::make_MAIN();}
"uint"                 {return yy::parser::make_UINT();}
[ \t\n] ;
"//".* ;
[0-9]+                 {
    Node* node = new Node(INTNUMBER_AST,yytext);
    return yy::parser::make_INTNUMBER(node);
}
[a-zA-Z_][0-9a-zA-Z_]* {
    Node* node = new Node(IDENT_AST,yytext);
    return yy::parser::make_IDENT(node);
}

%%
%{
#include "all_node.hpp"
#include "driver.hpp"
#include "parser.hpp"

%}

%option noyywrap
%option header-file="scanner.hpp"

%{
%}

%%
"=="                   {return yy::parser::make_EQUAL();}
"!="                   {return yy::parser::make_NOTEQUAL();}
"<"                    {return yy::parser::make_LESS();}
"<="                   {return yy::parser::make_LESSEQUAL();}
">"                    {return yy::parser::make_GREATER();}
">="                   {return yy::parser::make_GREATEREQUAL();}
"+"                    {return yy::parser::make_PLUS();}
"-"                    {return yy::parser::make_MINUS();}
"*"                    {return yy::parser::make_TIMES();}
"/"                    {return yy::parser::make_DIVIDE();}
"%"                    {return yy::parser::make_MOD();}
"("                    {return yy::parser::make_LPAREN();}
")"                    {return yy::parser::make_RPAREN();}
"{"                    {return yy::parser::make_LBRACE();}
"}"                    {return yy::parser::make_RBRACE();}
","                    {return yy::parser::make_COMMA();}
";"                    {return yy::parser::make_SEMI();}
"="                    {return yy::parser::make_ASSIGN();}
"fn"                   {return yy::parser::make_FN();}
"main"                 {return yy::parser::make_MAIN();}
"if"                   {return yy::parser::make_IF();}
"else"                 {return yy::parser::make_ELSE();}
"while"                {return yy::parser::make_WHILE();}
"uint"                 {return yy::parser::make_UINT();}
"int"                  {return yy::parser::make_INT();}
"fixed"                {return yy::parser::make_FIXED();}
"bool"                 {return yy::parser::make_BOOL();}
"char"                 {return yy::parser::make_CHAR();}
"scan"                 {return yy::parser::make_SCAN();}
"print"                {return yy::parser::make_PRINT();}
[ \t\n] ;
"//".* ;
[0-9]+                 {
    Node* node = new IntNumber(yytext);
    return yy::parser::make_INTNUMBER(node);
}
[a-zA-Z_][0-9a-zA-Z_]* {
    Node* node = new Ident(yytext);
    return yy::parser::make_IDENT(node);
}

%%
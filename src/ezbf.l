%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "ezbf.tab.h"

int yywrap(void) {
    return 1;
}
%}
%%
"=="                   {return EQUAL;}
"!="                   {return NOTEQUAL;}
"<"                    {return LESS;}
"<="                   {return LESSEQUAL;}
">"                    {return GREATER;}
">="                   {return GREATEREQUAL;}
"+"                    {return PLUS;}
"-"                    {return MINUS;}
"*"                    {return TIMES;}
"/"                    {return DIVIDE;}
"%"                    {return MOD;}
"("                    {return LPAREN;}
")"                    {return RPAREN;}
"{"                    {return LBRACE;}
"}"                    {return RBRACE;}
","                    {return COMMA;}
";"                    {return SEMI;}
"="                    {return ASSIGN;}
"fn"                   {return FN;}
"main"                 {return MAIN;}
"if"                   {return IF;}
"else"                 {return ELSE;}
"while"                {return WHILE;}
"uint"                 {return UINT;}
"int"                  {return INT;}
"fixed"                {return FIXED;}
"bool"                 {return BOOL;}
"char"                 {return CHAR;}
"scan"                 {return SCAN;} 
"print"                {return PRINT;}
[ \t\n] ;
"//".* ;
[0-9]+                 {
    yylval.p = buildStrNode(INTNUMBER_AST,yytext);
    return INTNUMBER;
}
\'.\'                  {
    char *s;
    s = (char *)malloc(3);
    sprintf(s,"%d",yytext[1]);
    yylval.p = buildStrNode(INTNUMBER_AST,s);
    return INTNUMBER;
}
\".+\"                 {
    char *s;
    s = (char *)malloc(sizeof(char)*yyleng-1);
    memcpy(s,&yytext[1],yyleng-2);
    // char s[256];
    // for (int i = 1;i < yyleng-1;++i) s[i-1] = yytext[i];
    s[yyleng-2] = '\0';
    yylval.p = buildStrNode(STR_AST,s);
    return STR;
}
[0-9]+\.[0-9]*         {
    yylval.p = buildStrNode(DECIMALNUMBER_AST,yytext);
    return DECIMALNUMBER;
}
[a-zA-Z_][0-9a-zA-Z_]* {
    yylval.p = buildStrNode(IDENT_AST,yytext);
    return IDENT;
}
    /* "true"        {return TRUE;} */
    /* "false"       {return FALSE;} */
    /* "&&"          {return AND;} */
    /* "||"          {return OR;} */
    /* "["           {return LBRACKET;} */
    /* "]"           {return RBRACKET;} */
    /* "|"           {return BAR;} */
    /* <<EOF>>       {return EOF;} */
%%
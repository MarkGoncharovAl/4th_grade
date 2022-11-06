%{

//------------------------------------------------------------------------------
//
// scanner.l -- lexical rules for ParaCL v2
//
//------------------------------------------------------------------------------

#include "/home/mark/Programming/7/ParProga/MIPT/Compiler/Language/parser.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void ScanError(char const *, char const *, int lineno);

%}

id          [a-zA-Z][a-zA-Z_0-9]*
data        ["][^"]*["]
ws          [ \t]+
int         [0-9]+
%option yylineno

%%
"\n"        {  yyinlinePos = 0; }
"\r"        {}
{ws}        {  BeginToken(yytext, &yyinlinePos); }
"def"       {
               BeginToken(yytext, &yyinlinePos);
               return DEF;
            }   
"if"        {
               BeginToken(yytext, &yyinlinePos);
               return IF;
            }   
"while"     {
               BeginToken(yytext, &yyinlinePos);
               return WHILE;
            }
"print"     {
               BeginToken(yytext, &yyinlinePos);
               return OUTPUT;
            }
"ret"       {
               BeginToken(yytext, &yyinlinePos);
               return RET;
            }
"init"      {
               BeginToken(yytext, &yyinlinePos);
               return INITARR;
            }
"set"       {
               BeginToken(yytext, &yyinlinePos);
               return SET;
            }
"get"       {
               BeginToken(yytext, &yyinlinePos);
               return GET;
            }
"+"         {  BeginToken(yytext, &yyinlinePos); return PLUS;  }
"-"         {  BeginToken(yytext, &yyinlinePos); return MINUS;  }
"*"         {  BeginToken(yytext, &yyinlinePos); return MUL;  }
"/"         {  BeginToken(yytext, &yyinlinePos); return DIV;  }
"("         {  BeginToken(yytext, &yyinlinePos); return LPAR;  }
")"         {  BeginToken(yytext, &yyinlinePos); return RPAR;  }
"["         {  BeginToken(yytext, &yyinlinePos); return LBR;  }
"]"         {  BeginToken(yytext, &yyinlinePos); return RBR;  }
"{"         {  BeginToken(yytext, &yyinlinePos); return LCB;  }
"}"         {  BeginToken(yytext, &yyinlinePos); return RCB;  }
"|"         {  BeginToken(yytext, &yyinlinePos); return UP;  }
";"         {  BeginToken(yytext, &yyinlinePos); return SM;  }
"="         {  BeginToken(yytext, &yyinlinePos); return ASG;  }
"@"         {  BeginToken(yytext, &yyinlinePos); return DOG;  }
"=="        {  BeginToken(yytext, &yyinlinePos);  
               yylval.set(Ops::Equal);
               return RELOP;
            }
"!="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.set(Ops::NotEqual);
               return RELOP;
            }
"<"         {
               BeginToken(yytext, &yyinlinePos);
               yylval.set(Ops::Less);
               return RELOP;
            }
">"         {
               BeginToken(yytext, &yyinlinePos);
               yylval.set(Ops::Greater);
               return RELOP;
            }
"<="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.set(Ops::LessEq);
               return RELOP;
            }
">="        {
               BeginToken(yytext, &yyinlinePos);
               yylval.set(Ops::GreaterEq);
               return RELOP;
            }
"||"        {  BeginToken(yytext, &yyinlinePos); return OR;  }
"&&"        {  BeginToken(yytext, &yyinlinePos); return AND;  }
"!"         {  BeginToken(yytext, &yyinlinePos); return NOT;  }
"?"         {  BeginToken(yytext, &yyinlinePos); return INPUT; }

{id}        {
               BeginToken(yytext, &yyinlinePos);
               yylval.name = yytext;
               return NAME;
            }
{data}      {
               BeginToken(yytext, &yyinlinePos);
               std::string dat(yytext);
               yylval.set(dat);
               return DATA;
            }
{int}       {
               BeginToken(yytext, &yyinlinePos);
               yylval.set(atoi(yytext));
               return VALUE;
            }
.           {  
               BeginToken(yytext, &yyinlinePos);
               ScanError("Unexpected smth", yytext, yylineno);
               return ILLEGAL;  
            }
%%

int yywrap() { return 1; }

void ScanError(char const * s, char const * yytext, int lineno) {
  fprintf(stdout, "Scanner error: %s in line %d\n", s, lineno);
  fprintf(stdout, "%s\n", yytext);
}

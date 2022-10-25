%{

//------------------------------------------------------------------------------
//
// compiler.y -- grammar rules for ParaCL v2
//
//------------------------------------------------------------------------------

#include "/home/mark/Programming/7/ParProga/MIPT/Compiler/Language/parser.hpp"
#include <iostream>

%}

%defines

%token NAME VALUE
%token WHILE IF DEF
%token SM LBR RBR LPAR RPAR LCB RCB ILLEGAL
%token OUTPUT
%token DATA
%right ASG
%nonassoc RELOP
%left INPUT
%left OR
%left AND
%left NOT
%left PLUS MINUS
%left MUL DIV MOD
%nonassoc UMINUS

%%

program:      stms                    { wholeProgramAction(); }

scope:        open_sc stms close_sc   { $$ = $3; }

open_sc:      LCB                     { currentScope = currentScope->push(); }

close_sc:     RCB                     {
                                        $$.treeNode = currentScope;
                                        currentScope = currentScope->resetScope();
                                      }
            
stms:         stm                     { currentScope->addBranch($1.treeNode); }
            | stms stm                { currentScope->addBranch($2.treeNode); }
            | stms scope              { currentScope->addBranch($2.treeNode); }

stm:          oper                    { $$ = $1; }

oper:         assign                  { $$ = $1; }
            | def                     { $$ = $1; }
            | if                      { $$ = $1; }
            | while                   { $$ = $1; }
            | output                  { $$ = $1; }

assign:       lval ASG expr SM        { $$.treeNode = make_op($1.treeNode, Ops::Assign, $3.treeNode); }

lval:         NAME                    { $$.treeNode = currentScope->access($1.name); }
              
expr:         expr PLUS expr          { $$.treeNode = make_op($1.treeNode, Ops::Plus, $3.treeNode); }
            | expr MINUS expr         { $$.treeNode = make_op($1.treeNode, Ops::Minus, $3.treeNode); }
            | expr MUL expr           { $$.treeNode = make_op($1.treeNode, Ops::Mul, $3.treeNode); }
            | expr DIV expr           { $$.treeNode = make_op($1.treeNode, Ops::Div, $3.treeNode); }
            | MINUS expr %prec UMINUS { $$.treeNode = make_op(make_value(0), Ops::Minus, $2.treeNode); }
            | NAME                    { 
                                        $$.treeNode = currentScope->find($1.name);
                                        if (!$$.treeNode) {
                                          YYLTYPE * info = &@1;
                                          PrintError("Using undeclared variable");
                                        }
                                      }
            | VALUE                   { $$.treeNode = make_value($1.getInt()); }
            | INPUT                   { $$.treeNode = make_op(nullptr, Ops::StdIn, nullptr); }
            | LPAR expr RPAR          { $$.treeNode = $2.treeNode; }

if:           IF lexpr scope          {
                                        $$.treeNode = make_if($2.treeNode, $3.treeNode);
                                      }

def:          DEF NAME scope          {
                                        $$.treeNode = make_def($2.name, $3.treeNode);
                                        currentScope->add($2.name, $$.treeNode);
                                      }

lexpr:        expr                    { $$ = $1; }
            | expr RELOP expr         { $$.treeNode = make_op($1.treeNode, $2.getOp(), $3.treeNode); }
            | lexpr OR lexpr          { $$.treeNode = make_op($1.treeNode, Ops::Or, $3.treeNode); }
            | lexpr AND lexpr         { $$.treeNode = make_op($1.treeNode, Ops::And, $3.treeNode); }
            | NOT lexpr               { $$.treeNode = make_op(nullptr, Ops::Not, $2.treeNode); }
            | LPAR lexpr RPAR         { $$ = $2; }

while:        WHILE lexpr scope       { 
                                        $$.treeNode = make_while($2.treeNode, $3.treeNode);
                                      }

output:       OUTPUT expr SM          { $$.treeNode = make_op(nullptr, Ops::StdOut, $2.treeNode); }
            | OUTPUT DATA SM          { $$.treeNode = make_op(nullptr, Ops::StdOut, $2.treeNode); }

%%
    
int yyerror(const char *s) {
  PrintError(s);
  return 0;
}

int wholeProgramAction() {
  currentScope->codegen();
  return 0;
}
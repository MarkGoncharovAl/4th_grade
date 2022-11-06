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
%token OUTPUT RET UP
%token DATA DOG INITARR SET GET
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
open_up:      UP                      { currentScope = currentScope->push(); }

close_sc:     RCB                     {
                                        $$.treeNode = currentScope;
                                        currentScope = currentScope->resetScope();
                                      }
            
stms:         stm                     { currentScope->addBranch($1.treeNode); }
            | stms stm                { currentScope->addBranch($2.treeNode); }
            | stms scope              { currentScope->addBranch($2.treeNode); }

stm:          oper                    { $$ = $1; }

oper:         assign                  { $$ = $1; }
            | defStat                 { $$ = $1; }
            | DOG initArr DOG         { $$ = $2; }
            | DOG setArr  DOG         { $$ = $2; }
            | if                      { $$ = $1; }
            | while                   { $$ = $1; }
            | output                  { $$ = $1; }
            | ret                     { $$ = $1; }

assign:       lval ASG expr SM        { $$.treeNode = createNode<Op>($1.treeNode, Ops::Assign, $3.treeNode); }

initArr:      INITARR NAME VALUE      {
                                        $$.treeNode = createNode<DeclArr>($3.getInt(), $2.name);
                                        currentScope->addArray($2.name, $$.treeNode);
                                      }

setArr:       SET NAME expr expr      {
                                        $$.treeNode = createNode<DeclArrOp>(
                                          currentScope->findArr($2.name),
                                          Ops::Set,
                                          $3.treeNode,
                                          $4.treeNode);
                                      }

getArr:       GET NAME expr           {
                                        $$.treeNode = createNode<DeclArrOp>(
                                          currentScope->findArr($2.name),
                                          Ops::Get,
                                          $3.treeNode,
                                          nullptr);
                                      }

lval:         NAME                    { $$.treeNode = currentScope->access($1.name); }
              
expr:         expr PLUS expr          { $$.treeNode = createNode<Op>($1.treeNode, Ops::Plus, $3.treeNode); }
            | expr MINUS expr         { $$.treeNode = createNode<Op>($1.treeNode, Ops::Minus, $3.treeNode); }
            | expr MUL expr           { $$.treeNode = createNode<Op>($1.treeNode, Ops::Mul, $3.treeNode); }
            | expr DIV expr           { $$.treeNode = createNode<Op>($1.treeNode, Ops::Div, $3.treeNode); }
            | MINUS expr %prec UMINUS { $$.treeNode = createNode<Op>(createNode<Value>(0), Ops::Minus, $2.treeNode); }
            | NAME                    { 
                                        $$.treeNode = currentScope->find($1.name);
                                        if (!$$.treeNode) {
                                          YYLTYPE * info = &@1;
                                          PrintError("Using undeclared variable");
                                        }
                                      }
            | VALUE                   { $$.treeNode = createNode<Value>($1.getInt()); }
            | INPUT                   { $$.treeNode = createNode<Op>(nullptr, Ops::StdIn, nullptr); }
            | LPAR expr RPAR          { $$.treeNode = $2.treeNode; }
            | LBR NAME RBR            { 
                                        $$.treeNode = createNode<Call>($2.name, currentScope);
                                      }
            | LBR NAME exprs RBR      { 
                                        $$.treeNode = createNode<Call>($2.name, currentScope, $3.treeNode);
                                      }
            | DOG getArr DOG          {
                                        $$.treeNode = $2.treeNode;
                                      }

exprs:        expr                    { $$.treeNode = createNode<Storage>($1.treeNode); } 
            | expr exprs              { 
                                        cast<Storage>($2.treeNode)->add($1.treeNode); 
                                        $$.treeNode = $2.treeNode;
                                      }

if:           IF lexpr scope          {
                                        $$.treeNode = createNode<If>($2.treeNode, $3.treeNode, stackFunction.top());
                                      }

defStat:      def stms close_sc       {
                                        cast<Def>($1.treeNode)->setScope($3.treeNode);
                                        currentScope->addFunc($1.name, $1.treeNode);
                                        $$.treeNode = $1.treeNode;
                                        stackFunction.pop();
                                      }

def:          DEF NAME open_up names LCB
                                      {
                                        $$.treeNode = createNode<Def>($2.name, $4.treeNode);
                                        currentScope->access($4.treeNode, $$.treeNode);
                                        $$.name = $2.name;
                                        stackFunction.push(cast<Def>($$.treeNode)->getFunc());
                                      }
            | DEF NAME open_sc
                                      {
                                        $$.treeNode = createNode<Def>($2.name);
                                        $$.name = $2.name;
                                        stackFunction.push(cast<Def>($$.treeNode)->getFunc());
                                      }

ret:          RET expr SM             {
                                        $$.treeNode = createNode<Ret>($2.treeNode);
                                      }

lexpr:        expr                    { $$ = $1; }
            | expr RELOP expr         { $$.treeNode = createNode<Op>($1.treeNode, $2.getOp(), $3.treeNode); }
            | lexpr OR lexpr          { $$.treeNode = createNode<Op>($1.treeNode, Ops::Or, $3.treeNode); }
            | lexpr AND lexpr         { $$.treeNode = createNode<Op>($1.treeNode, Ops::And, $3.treeNode); }
            | NOT lexpr               { $$.treeNode = createNode<Op>(nullptr, Ops::Not, $2.treeNode); }
            | LPAR lexpr RPAR         { $$ = $2; }

while:        WHILE lexpr scope       { 
                                        $$.treeNode = createNode<While>($2.treeNode, $3.treeNode, stackFunction.top());
                                      }

output:       OUTPUT expr SM          { $$.treeNode = createNode<Op>(nullptr, Ops::StdOut, $2.treeNode); }
            | OUTPUT DATA SM          { $$.treeNode = createNode<Op>(nullptr, Ops::StdOut, $2.treeNode); }

names:        NAME                    { $$.treeNode = createNode<Names>($1.name); }
            | NAME names              { 
                                        cast<Names>($2.treeNode)->add($1.name); 
                                        $$.treeNode = $2.treeNode;
                                      }

%%
    
int yyerror(const char *s) {
  PrintError(s);
  return 0;
}

int wholeProgramAction() {
  currentScope->codegen();
  return 0;
}
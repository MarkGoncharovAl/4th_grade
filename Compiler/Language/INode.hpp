//------------------------------------------------------------------------------
//
// INode -- main node interfaces, operations enum and ctor functions for nodes
//
//------------------------------------------------------------------------------

#pragma once

#include "llvm/IR/Value.h"
#include <string>

// node interface
struct INode
{
  virtual llvm::Value* codegen () = 0;
  virtual void dump () const = 0;
  virtual ~INode () {}
};

// scope interface
struct IScope: public INode
{
  virtual IScope* push () = 0;
  virtual IScope* resetScope () const = 0;
  virtual void addBranch (INode* branch) = 0;
  virtual INode* access (std::string const& var_name) = 0;
  virtual INode* find (const std::string& symbol) = 0;
  virtual void add (std::string symbol , INode* n) = 0;
};

// operations
enum class Ops
{
  Plus ,
  Minus ,
  Assign ,
  Greater ,
  Less ,
  GreaterEq ,
  LessEq ,
  StdOut ,
  StdIn ,
  Equal ,
  NotEqual ,
  Div ,
  Mul ,
  Not ,
  And ,
  Or
};

// ctor functions
INode* make_value (int);
INode* make_op (INode* l , Ops o , INode* r);
INode* make_while (INode* o , INode* s);
INode* make_if (INode* o , INode* s);
INode* make_def (const std::string& name , INode* s);
IScope* create_scope ();

using Node = std::unique_ptr<INode*>;
//------------------------------------------------------------------------------
//
// INode -- main node interfaces, operations enum and ctor functions for nodes
//
//------------------------------------------------------------------------------

#pragma once

#include "llvm/IR/Value.h"
#include <string>

// node interface
struct INode {
  virtual llvm::Value *codegen() = 0;
  virtual void dump() const = 0;
  virtual ~INode() {}
};

// scope interface
struct IScope : public INode {
  virtual IScope *push() = 0;
  virtual IScope *resetScope() const = 0;
  virtual void addBranch(INode *branch) = 0;
  virtual INode *access(std::string const &var_name) = 0;
  virtual INode *access(INode *names, INode *func) = 0;
  virtual INode *find(const std::string &symbol) = 0;
  virtual INode *findFunc(const std::string &symbol) = 0;
  virtual void addFunc(std::string const &symbol, INode *n) = 0;
};

// operations
enum class Ops {
  Plus,
  Minus,
  Assign,
  Greater,
  Less,
  GreaterEq,
  LessEq,
  StdOut,
  StdIn,
  Equal,
  NotEqual,
  Div,
  Mul,
  Not,
  And,
  Or
};

// ctor functions
INode *make_value(int);
INode *make_op(INode *l, Ops o, INode *r);
INode *make_while(INode *o, INode *s, llvm::Function *func);
INode *make_if(INode *o, INode *s, llvm::Function *func);
INode *make_def(const std::string &name, INode *call_names);
INode *make_call(const std::string &name, IScope *scope, INode *storage);
INode *make_ret(INode *ret);
INode *make_names(std::string const &name);
INode *make_storage(INode *node);

void add_scope(INode *func, INode *sc);
INode *add_name(INode *names, std::string const &name);
INode *add_storage(INode *storage, INode *node);

IScope *create_scope();
llvm::Function *create_function(INode *defNode);

using Node = std::unique_ptr<INode *>;
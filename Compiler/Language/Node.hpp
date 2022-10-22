//------------------------------------------------------------------------------
//
// Node.hpp -- concrete node types
//
//------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "INode.hpp"

using RType = int;

// Integer value
class Value final: public INode
{
  int val;

  // INode interface
  public:
  llvm::Value* codegen () override;
  void dump () const override;

  public:
  Value (int v): val (v) {}
};

// Declaration
class Decl final: public INode
{
  int val;
  std::string varname;

  // INode interface
  public:
  Decl (const std::string& n): varname (n) {}
  llvm::Value* codegen () override;
  llvm::Value* assign (llvm::Value* RHS);
  void dump () const override;

  public:
  Decl () = default;
  void SetValue (int Val);
};

// Scope
class Scope final: public IScope
{
  std::vector<INode*> branches;
  std::unordered_map<std::string , INode*> symbols_;

  IScope* prev_scope;

  // INode interface
  public:
  llvm::Value* codegen () override;
  void dump () const override;

  // IScope interface
  public:
  IScope* push () override { return new Scope (this); }
  IScope* resetScope () const override;
  void addBranch (INode* branch) override;
  INode* access (std::string const& var_name) override;
  INode* visible (std::string const& var_name) override;

  void add (std::string symbol , INode* n) { symbols_[symbol] = n; }
  INode* find (std::string symbol)
  {
    auto it = symbols_.find (symbol);
    if (it == symbols_.end ())
      return nullptr;
    return it->second;
  }

  public:
  Scope (Scope* prev): prev_scope (prev) {}
  ~Scope ();
};

// Operand
class Op final: public INode
{
  INode* left;
  INode* right;
  Ops op;

  // INode interface
  public:
  llvm::Value* codegen () override;
  void dump () const override;

  public:
  Op (INode* l , Ops o , INode* r): left (l) , right (r) , op (o) {}
  ~Op ();
};

// While loop node
class While final: public INode
{
  INode* op = nullptr;
  INode* scope = nullptr;

  // INode interface
  public:
  llvm::Value* codegen () override;
  void dump () const override;

  public:
  While (INode* o , INode* s): op (o) , scope (s) {}
  ~While ();
};

// If node
class If final: public INode
{
  INode* cmp;
  INode* scope;
  llvm::BasicBlock* block_true = nullptr;
  llvm::BasicBlock* block_out = nullptr;

  public:
  llvm::Value* codegen () override;
  void dump () const override;

  If (INode* def_cmp , INode* def_scope)
    : cmp (def_cmp) ,
    scope (def_scope)
  {}
  ~If ()
  {
    //if (cmp && typeid(Decl) != typeid(*cmp))
    //  delete cmp;
    //delete scope;
  }
};

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
  INode* scope;

  // INode interface
  public:
  Decl (const std::string& n , INode* def_scope): varname (n) ,
    scope (def_scope)
  {}
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
  std::unordered_map<std::string , INode*> funcs_;
  std::unordered_map<std::string , llvm::Value*> allocs_;

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

  void add (std::string symbol , INode* n) override;
  INode* find (const std::string& symbol) override;
  llvm::Value* findAlloc (const std::string& symbol);

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
  INode* cmp;
  INode* scope;

  public:
  llvm::Value* codegen () override;
  void dump () const override;

  While (INode* def_cmp , INode* def_scope)
    : cmp (def_cmp) ,
    scope (def_scope)
  {}
  ~While () {};
};

// If node
class If final: public INode
{
  INode* cmp;
  INode* scope;

  public:
  llvm::Value* codegen () override;
  void dump () const override;

  If (INode* def_cmp , INode* def_scope)
    : cmp (def_cmp) ,
    scope (def_scope)
  {}
  ~If () {}
};

class Def final: public INode
{
  std::string name;
  INode* scope;
  llvm::Function* func;

  public:
  llvm::Value* codegen () override;
  void dump () const override;

  Def (const std::string& def_name , INode* def_scope);
  ~Def () {}
};
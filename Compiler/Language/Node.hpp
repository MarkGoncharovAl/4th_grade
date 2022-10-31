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
class Value final : public INode {
  int val;

public:
  llvm::Value *codegen() override;
  void dump() const override;

public:
  Value(int v) : val(v) {}
};
class Storage final : public INode {
  std::vector<INode *> data;
  std::vector<llvm::Value *> values;

public:
  llvm::Value *codegen() override {
    for (auto node : data)
      values.push_back(node->codegen());
    return nullptr;
  }
  void dump() const override {
    std::cout << "Storage:\n";
    for (auto node : data)
      node->dump();
  }
  void add(INode *node) { data.insert(data.begin(), node); }
  llvm::ArrayRef<llvm::Value *> getValues() const {
    return llvm::ArrayRef(values);
  }

public:
  Storage(INode *node) : data{node} {}
};
class LLVMValue final : public INode {
  llvm::Value *val;

public:
  llvm::Value *codegen() override { return val; }
  void dump() const override { val->print(llvm::errs()); }

public:
  LLVMValue(llvm::Value *def_val) : val(def_val) {}
};

class Names final : public INode {
  std::vector<std::string> names;

public:
  llvm::Value *codegen() override;
  void dump() const override;
  void add(std::string const &name) { names.insert(names.begin(), name); }
  const std::vector<std::string> &get() const { return names; }

public:
  Names(std::string const &name) : names{name} {}
};

class Ret final : public INode {
  INode *ret = nullptr;

public:
  llvm::Value *codegen() override;
  void dump() const override;

public:
  Ret(INode *rhs) : ret(rhs) {}
};

// Declaration
class Decl final : public INode {
  bool first_time = true;
  int val;
  std::string varname;
  llvm::Value *alloca = nullptr;

  // INode interface
public:
  Decl(const std::string &n) : varname(n) {}
  llvm::Value *codegen() override;
  llvm::Value *assign(llvm::Value *RHS);
  void dump() const override;
  llvm::Value *getAlloc() const;

public:
  Decl() = default;
  void SetValue(int Val);
};

// Scope
class Scope final : public IScope {
  std::vector<INode *> branches;
  std::unordered_map<std::string, INode *> symbols_;
  std::unordered_map<std::string, INode *> funcs_;

  IScope *prev_scope;

  // INode interface
public:
  llvm::Value *codegen() override;
  void dump() const override;

  // IScope interface
public:
  IScope *push() override { return new Scope(this); }
  IScope *resetScope() const override;
  void addBranch(INode *branch) override;
  void addFunc(const std::string &name, INode *func) override;
  INode *access(std::string const &var_name) override;
  INode *access(INode *names, INode *func) override;
  INode *find(const std::string &symbol) override;
  INode *findFunc(const std::string &symbol) override;

public:
  Scope(Scope *prev) : prev_scope(prev) {}
  ~Scope();
};

// Operand
class Op final : public INode {
  INode *left;
  INode *right;
  Ops op;

  // INode interface
public:
  llvm::Value *codegen() override;
  void dump() const override;

public:
  Op(INode *l, Ops o, INode *r) : left(l), right(r), op(o) {}
  ~Op();
};

// While loop node
class While final : public INode {
  INode *cmp;
  INode *scope;
  llvm::Function *func;

public:
  llvm::Value *codegen() override;
  void dump() const override;

  While(INode *def_cmp, INode *def_scope, llvm::Function *def_func)
      : cmp(def_cmp), scope(def_scope), func(def_func) {}
  ~While(){};
};

// If node
class If final : public INode {
  INode *cmp;
  INode *scope;
  llvm::Function *func;

public:
  llvm::Value *codegen() override;
  void dump() const override;

  If(INode *def_cmp, INode *def_scope, llvm::Function *def_func)
      : cmp(def_cmp), scope(def_scope), func(def_func) {}
  ~If() {}
};

class Def final : public INode {
  std::string name;
  IScope *scope;
  llvm::Function *func;

public:
  llvm::Value *codegen() override;
  llvm::Function *getFunc() const { return func; }
  void dump() const override;
  void setScope(INode *def_scope) { scope = static_cast<IScope *>(def_scope); }

  Def(const std::string &def_name, INode *call_names);
  ~Def() {}
};

class Call final : public INode {
  std::string name;
  IScope *scope;
  Storage *args;

public:
  Call(std::string const &name_func, IScope *def_scope, INode *def_args)
      : name(name_func), scope(def_scope),
        args(static_cast<Storage *>(def_args)) {}
  llvm::Value *codegen() override;
  void dump() const override;
};
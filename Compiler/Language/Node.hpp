//------------------------------------------------------------------------------
//
// Node.hpp -- concrete node types
//
//------------------------------------------------------------------------------

#pragma once

#include "enums.h"
#include "llvm/ADT/APInt.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Value.h"
#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

// --------- Interface ------------------------------------
struct INode {
  static constexpr Types getTypeClass() { return Types::INode; }
  virtual constexpr Types getType() const noexcept = 0;
  virtual llvm::Value *codegen() = 0;
  virtual void dump() const = 0;
  virtual ~INode() {}
};

struct IScope : public INode {
  static constexpr Types getTypeClass() { return Types::IScope; }
  virtual constexpr Types getType() const noexcept = 0;
  virtual IScope *push() = 0;
  virtual IScope *resetScope() const = 0;
  virtual void addBranch(INode *branch) = 0;
  virtual void addArray(std::string const &symbol, INode *branch) = 0;
  virtual INode *access(std::string const &var_name) = 0;
  virtual INode *access(INode *names, INode *func) = 0;
  virtual INode *find(const std::string &symbol) = 0;
  virtual INode *findFunc(const std::string &symbol) = 0;
  virtual INode *findArr(const std::string &symbol) = 0;
  virtual void addFunc(std::string const &symbol, INode *n) = 0;
};
// --------------------------------------------------------
// --------- Cast -----------------------------------------

template <typename T> concept _castConcept_ = requires(T &&obj) {
  {T::getTypeClass()};
};
template <_castConcept_ T> T *dyn_cast(INode *node) {
  if (node->getType() != T::getTypeClass())
    return nullptr;
  return static_cast<T *>(node);
}
template <_castConcept_ T> T *cast(INode *node) {
  assert(node->getType() == T::getTypeClass() && "Can't cast types!");
  return static_cast<T *>(node);
}
template <_castConcept_ T, typename... In> INode *createNode(In... vars) {
  return new T(vars...);
}

using RType = int;

// --------------------------------------------------------
// --------- Realization ----------------------------------
class Value final : public INode {
  int val;

public:
  static constexpr Types getTypeClass() { return Types::Value; }
  constexpr Types getType() const noexcept override { return Types::Value; }
  llvm::Value *codegen() override;
  void dump() const override;
  Value(int v) : val(v) {}
};

class Storage final : public INode {
  std::vector<INode *> data;
  std::vector<llvm::Value *> values;

public:
  static constexpr Types getTypeClass() { return Types::Storage; }
  constexpr Types getType() const noexcept override { return Types::Storage; }
  llvm::Value *codegen() override;
  void dump() const override;
  void add(INode *node);
  llvm::ArrayRef<llvm::Value *> getValues() const;
  Storage(INode *node) : data{node} {}
};

class LLVMValue final : public INode {
  llvm::Value *val;

public:
  static constexpr Types getTypeClass() { return Types::LLVMValue; }
  constexpr Types getType() const noexcept override { return Types::LLVMValue; }
  llvm::Value *codegen() override;
  void dump() const override;
  LLVMValue(llvm::Value *def_val) : val(def_val) {}
};

class Names final : public INode {
  std::vector<std::string> names;

public:
  static constexpr Types getTypeClass() { return Types::Names; }
  constexpr Types getType() const noexcept override { return Types::Names; }
  llvm::Value *codegen() override;
  void dump() const override;
  void add(std::string const &name) { names.insert(names.begin(), name); }
  const std::vector<std::string> &get() const { return names; }
  Names(std::string const &name) : names{name} {}
};

class Ret final : public INode {
  INode *ret = nullptr;

public:
  static constexpr Types getTypeClass() { return Types::Ret; }
  constexpr Types getType() const noexcept override { return Types::Ret; }
  llvm::Value *codegen() override;
  void dump() const override;

public:
  Ret(INode *rhs) : ret(rhs) {}
};

class Decl final : public INode {
  bool first_time = true;
  int val;
  std::string varname;
  llvm::Value *alloca = nullptr;

public:
  static constexpr Types getTypeClass() { return Types::Decl; }
  constexpr Types getType() const noexcept override { return Types::Decl; }
  Decl(const std::string &n) : varname(n) {}
  llvm::Value *codegen() override;
  llvm::Value *assign(llvm::Value *RHS);
  void dump() const override;
  llvm::Value *getAlloc() const;
  Decl() = default;
  void SetValue(int Val);
};

class DeclArr final : public INode {
  int size;
  std::string name;
  llvm::Value *alloca = nullptr;

public:
  static constexpr Types getTypeClass() { return Types::DeclArr; }
  constexpr Types getType() const noexcept override { return Types::DeclArr; }
  DeclArr(int def_size, std::string const &def_name);

  llvm::Value *codegen() override;
  llvm::Value *store(llvm::Value *idxVal, llvm::Value *storeVal);
  llvm::Value *load(llvm::Value *idxVal);

  void dump() const override;
};

class DeclArrOp final : public INode {
  Ops op;
  DeclArr *decl;
  INode *idx;
  INode *store;

public:
  static constexpr Types getTypeClass() { return Types::DeclArrOp; }
  constexpr Types getType() const noexcept override { return Types::DeclArrOp; }
  DeclArrOp(INode *def_decl, Ops def_op, INode *def_idx, INode *def_store);

  llvm::Value *codegen() override;
  void dump() const override;
};

// Scope
class Scope final : public IScope {
  std::vector<INode *> branches;
  std::unordered_map<std::string, INode *> symbols_;
  std::unordered_map<std::string, INode *> arrays_;
  std::unordered_map<std::string, INode *> funcs_;

  IScope *prev_scope;

  // INode interface
public:
  static constexpr Types getTypeClass() { return Types::Scope; }
  constexpr Types getType() const noexcept override { return Types::Scope; }
  llvm::Value *codegen() override;
  void dump() const override;

  // IScope interface
public:
  IScope *push() override { return new Scope(this); }
  IScope *resetScope() const override;
  void addBranch(INode *branch) override;
  void addFunc(const std::string &name, INode *func) override;
  void addArray(std::string const &symbol, INode *decl) override;
  INode *access(std::string const &var_name) override;
  INode *access(INode *names, INode *func) override;
  INode *find(const std::string &symbol) override;
  INode *findFunc(const std::string &symbol) override;
  INode *findArr(const std::string &symbol) override;

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
  static constexpr Types getTypeClass() { return Types::Op; }
  constexpr Types getType() const noexcept override { return Types::Op; }
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
  static constexpr Types getTypeClass() { return Types::While; }
  constexpr Types getType() const noexcept override { return Types::While; }
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
  static constexpr Types getTypeClass() { return Types::If; }
  constexpr Types getType() const noexcept override { return Types::If; }
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
  static constexpr Types getTypeClass() { return Types::Def; }
  constexpr Types getType() const noexcept override { return Types::Def; }
  llvm::Value *codegen() override;
  llvm::Function *getFunc() const { return func; }
  void dump() const override;
  void setScope(INode *def_scope) { scope = static_cast<IScope *>(def_scope); }

  Def(const std::string &def_name, INode *call_names = nullptr);
  ~Def() {}
};

class Call final : public INode {
  std::string name;
  IScope *scope;
  Storage *args;

public:
  static constexpr Types getTypeClass() { return Types::Call; }
  constexpr Types getType() const noexcept override { return Types::Call; }
  Call(std::string const &name_func, IScope *def_scope, INode *def_args = nullptr)
      : name(name_func), scope(def_scope),
        args(static_cast<Storage *>(def_args)) {}
  llvm::Value *codegen() override;
  void dump() const override;
};
//------------------------------------------------------------------------------
//
// Node.cpp -- concrete node types implementation
//
//------------------------------------------------------------------------------

#include "llvm/ADT/APInt.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"
#include <iostream>
#include <stack>

#include "Node.hpp"

extern llvm::LLVMContext *Context;
extern llvm::IRBuilder<> *Builder;
extern std::stack<llvm::Function *> stackFunction;
extern llvm::Module *Module;

// ----------------------------------------------------
// ------ Value ---------------------------------------
llvm::Value *Value::codegen() {
  return llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Context), val);
}
void Value::dump() const { std::cout << "Node Value: " << val << std::endl; }

// ----------------------------------------------------
// ------ Storage -------------------------------------
llvm::Value *Storage::codegen() {
  for (auto node : data)
    values.push_back(node->codegen());
  return nullptr;
}
void Storage::dump() const {
  std::cout << "Storage:\n";
  for (auto node : data)
    node->dump();
}
void Storage::add(INode *node) { data.insert(data.begin(), node); }
llvm::ArrayRef<llvm::Value *> Storage::getValues() const {
  return llvm::ArrayRef(values);
}

// ----------------------------------------------------
// ------ LLVMValue -----------------------------------
llvm::Value *LLVMValue::codegen() { return val; }
void LLVMValue::dump() const { val->print(llvm::errs()); }

// ----------------------------------------------------
// ------ Names ---------------------------------------
llvm::Value *Names::codegen() {
  assert(0 && "Code of class Names must not be called!");
}
void Names::dump() const {
  std::cout << "Names class has names: ";
  for (const auto &name : names)
    std::cout << name << " ";
  std::cout << "\n";
}

// ----------------------------------------------------
// ------ Decl ----------------------------------------
llvm::Value *Decl::codegen() {
  if (first_time) {
    first_time = false;
    alloca = Builder->CreateAlloca(llvm::Type::getInt32Ty(*Context), 0,
                                   varname.c_str());
  }
  assert(alloca && "Variable wasn't found!");
  return Builder->CreateLoad(alloca, varname.c_str());
}
llvm::Value *Decl::assign(llvm::Value *RHS) {
  assert(alloca && "Variable wasn't found!");
  return Builder->CreateStore(RHS, alloca);
}

void Decl::dump() const { std::cout << "Node Decl: " << val << std::endl; }
void Decl::SetValue(int Val) { val = Val; }

// ----------------------------------------------------
// ------ DeclArr -------------------------------------

DeclArr::DeclArr(int def_size, std::string const &def_name)
    : size(def_size), name(def_name) {}

llvm::Value *DeclArr::codegen() {
  alloca = Builder->CreateAlloca(
      llvm::ArrayType::get(Builder->getInt32Ty(), size), 0, name.c_str());
  return alloca;
}
llvm::Value *DeclArr::store(llvm::Value *idxVal, llvm::Value *storeVal) {
  llvm::Value *init = llvm::ConstantInt::get(Builder->getInt32Ty(), 0);
  llvm::Value *GEP =
      Builder->CreateGEP(alloca, llvm::ArrayRef<llvm::Value *>{init, idxVal});
  return Builder->CreateStore(storeVal, GEP);
}
llvm::Value *DeclArr::load(llvm::Value *idxVal) {
  llvm::Value *init = llvm::ConstantInt::get(Builder->getInt32Ty(), 0);
  llvm::Value *GEP =
      Builder->CreateGEP(alloca, llvm::ArrayRef<llvm::Value *>{init, idxVal});
  return Builder->CreateLoad(GEP);
}
void DeclArr::dump() const {
  std::cout << "Declration of array. Size: " << size << " Name: " << name
            << "\n";
}

// ----------------------------------------------------
// ------ DeclArrOp -----------------------------------

DeclArrOp::DeclArrOp(INode *def_decl, Ops def_op, INode *def_idx,
                     INode *def_store)
    : op(def_op), decl(cast<DeclArr>(def_decl)), idx(def_idx),
      store(def_store) {
  assert(idx && "Value must have valid idx!");
}

llvm::Value *DeclArrOp::codegen() {
  llvm::Value *idxVal = idx->codegen();
  switch (op) {
  case Ops::Get:
    return decl->load(idxVal);
  case Ops::Set:
    return decl->store(idxVal, store->codegen());
  default:
    break;
  };
  assert(0 &&
         "Operation can't be produced with array! Allow only get and store!");
}
void DeclArrOp::dump() const {
  std::cout << "Declaration operation node\n";
  decl->dump();
}

// ----------------------------------------------------
// ------ Scope ---------------------------------------
llvm::Value *Scope::codegen() {
  for (auto x : branches)
    x->codegen();
  return nullptr;
}

void Scope::dump() const {
  std::cout << "Node Scope: " << std::endl;
  for (auto const x : branches)
    x->dump();
}
IScope *Scope::resetScope() const { return prev_scope; }
void Scope::addBranch(INode *branch) { branches.push_back(branch); }

INode *Scope::access(std::string const &var_name) {
  INode *t = find(var_name);
  if (t)
    return t;

  INode *d = new Decl{var_name};
  symbols_[var_name] = d;
  return d;
}

INode *Scope::access(INode *names, INode *func) {
  auto namesData = cast<Names>(names);
  llvm::Function *function = cast<Def>(func)->getFunc();
  auto args = function->arg_begin();
  auto namesStr = namesData->get();
  for (auto iter = namesStr.cbegin(); iter != namesStr.cend(); ++iter, ++args) {
    Decl *decl = cast<Decl>(access(*iter));
    addBranch(decl);
    llvm::Value *argument = static_cast<llvm::Value *>(args);
    addBranch(createNode<Op>(decl, Ops::Assign, new LLVMValue(argument)));
  }
  return names;
}

INode *Scope::find(const std::string &symbol) {
  auto it = symbols_.find(symbol);
  if (it == symbols_.end()) {
    if (prev_scope != nullptr)
      return prev_scope->find(symbol);
    else
      return nullptr;
  }
  return it->second;
}

INode *Scope::findFunc(const std::string &symbol) {
  auto it = funcs_.find(symbol);
  if (it == funcs_.end()) {
    if (prev_scope != nullptr)
      return prev_scope->findFunc(symbol);
    else
      return nullptr;
  }
  return it->second;
}

INode *Scope::findArr(const std::string &symbol) {
  auto it = arrays_.find(symbol);
  if (it == arrays_.end()) {
    if (prev_scope != nullptr)
      return prev_scope->findArr(symbol);
    else
      return nullptr;
  }
  return it->second;
}

void Scope::addFunc(const std::string &name, INode *func) {
  assert(!findFunc(name) && "Can't add the same symbol to the table!");
  funcs_[name] = func;
}

void Scope::addArray(std::string const &symbol, INode *decl) {
  assert(!findArr(symbol) && "Can't add the same symbol to the table!");
  arrays_[symbol] = decl;
}

Scope::~Scope() {
  for (auto x : branches)
    delete x;
}

// ----------------------------------------------------
// ------ Ret -----------------------------------------
llvm::Value *Ret::codegen() {
  llvm::Value *retValue = ret->codegen();
  return Builder->CreateRet(retValue);
}
void Ret::dump() const { std::cout << "Return statement:\n"; }

// ----------------------------------------------------
// ------ Op -----------------------------------------
llvm::Value *Op::codegen() {
  llvm::Value *LeftV = nullptr;
  llvm::Value *RightV = nullptr;
  if (left)
    LeftV = left->codegen();
  if (right)
    RightV = right->codegen();
  switch (op) {
  case Ops::Plus:
    if (LeftV == nullptr)
      LeftV = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Context), 0);
    return Builder->CreateAdd(LeftV, RightV);
  case Ops::Minus:
    if (LeftV == nullptr)
      LeftV = llvm::ConstantInt::get(llvm::Type::getInt32Ty(*Context), 0);
    return Builder->CreateSub(LeftV, RightV);
  case Ops::Greater:
    return Builder->CreateICmpSGT(LeftV, RightV);
  case Ops::Less:
    return Builder->CreateICmpSLT(LeftV, RightV);
  case Ops::GreaterEq:
    return Builder->CreateICmpSGE(LeftV, RightV);
  case Ops::LessEq:
    return Builder->CreateICmpSLE(LeftV, RightV);
  case Ops::Assign:
    return cast<Decl>(left)->assign(RightV);
  case Ops::StdOut: {
    auto *CalleeF = Module->getFunction("__print_int");
    assert(CalleeF && "Driver shall create decl for __print_int");
    assert(RightV && "Print required non-null right arg");
    llvm::Value *ArgsV[] = {RightV};
    return Builder->CreateCall(CalleeF, ArgsV);
  }
  case Ops::StdIn: {
    auto *CalleeF = Module->getFunction("__scan_int");
    assert(CalleeF && "Driver shall create decl for __scan_int");
    return Builder->CreateCall(CalleeF);
  }
  case Ops::Equal:
    return Builder->CreateICmpEQ(LeftV, RightV);
  case Ops::NotEqual:
    return Builder->CreateICmpNE(LeftV, RightV);
  case Ops::Not:
    return Builder->CreateNot(RightV);
  case Ops::And:
    return Builder->CreateAnd(LeftV, RightV);
  case Ops::Or:
    return Builder->CreateOr(LeftV, RightV);
  case Ops::Div:
    return Builder->CreateSDiv(LeftV, RightV);
  case Ops::Mul:
    return Builder->CreateMul(LeftV, RightV);
  default:
    break;
  }

  // to implement
  return nullptr;
}

void Op::dump() const {
  std::cout << "Node Op: ";
  switch (op) {
  case Ops::Plus:
    std::cout << "Plus" << std::endl;
    break;
  case Ops::Minus:
    std::cout << "Minus" << std::endl;
    break;
  case Ops::Greater:
    std::cout << "Greater" << std::endl;
    break;
  case Ops::Less:
    std::cout << "Less" << std::endl;
    break;
  case Ops::Assign:
    std::cout << "Assign" << std::endl;
    break;
  case Ops::StdOut:
    std::cout << "StdOut" << std::endl;
    break;
  case Ops::StdIn:
    std::cout << "StdIn" << std::endl;
    break;
  case Ops::Equal:
    std::cout << "Equal" << std::endl;
    break;
  default:
    std::cout << "Unknown" << std::endl;
  }
  if (left != nullptr)
    left->dump();
  right->dump();
}

Op::~Op() {
  if (left && typeid(Decl) != typeid(*left))
    delete left;
  if (right && typeid(Decl) != typeid(*right))
    delete right;
}

// ----------------------------------------------------
// ------ While -----------------------------------------
llvm::Value *While::codegen() {
  llvm::BasicBlock *block_pre = llvm::BasicBlock::Create(*Context, "", func);
  llvm::BasicBlock *block_true = llvm::BasicBlock::Create(*Context, "", func);
  llvm::BasicBlock *block_out = llvm::BasicBlock::Create(*Context, "", func);

  Builder->CreateBr(block_pre);
  Builder->SetInsertPoint(block_pre);
  llvm::Value *ret = cmp->codegen();
  Builder->CreateCondBr(ret, block_true, block_out);

  Builder->SetInsertPoint(block_true);
  scope->codegen();
  if (!block_true->getTerminator())
    Builder->CreateBr(block_pre);
  Builder->SetInsertPoint(block_out);
  return ret;
}

void While::dump() const {
  std::cout << "Node While " << std::endl;
  scope->dump();
}

// ----------------------------------------------------
// ------ If -----------------------------------------
llvm::Value *If::codegen() {
  llvm::BasicBlock *block_true = llvm::BasicBlock::Create(*Context, "", func);
  llvm::BasicBlock *block_out = llvm::BasicBlock::Create(*Context, "", func);
  llvm::Value *ret = cmp->codegen();
  Builder->CreateCondBr(ret, block_true, block_out);
  Builder->SetInsertPoint(block_true);
  scope->codegen();
  if (!block_true->getTerminator ())
    Builder->CreateBr (block_out);
  Builder->SetInsertPoint(block_out);
  return ret;
}

void If::dump() const {
  std::cout << "Node If:\nCmp: ";
  cmp->dump();
  std::cout << "\nScope: ";
  scope->dump();
}

// ----------------------------------------------------
// ------ Def -----------------------------------------
Def::Def(const std::string &def_name, INode *call_names)
    : name(def_name), scope(nullptr), func(nullptr) {

  std::vector<llvm::Type*> types;
  if (call_names != nullptr) {
    size_t sizeNames = cast<Names>(call_names)->get().size();
    for (size_t i = 0; i < sizeNames; ++i)
      types.push_back(Builder->getInt32Ty());
  }

  llvm::FunctionType *FuncTy = llvm::FunctionType::get(
      Builder->getInt32Ty(), llvm::ArrayRef<llvm::Type *>(types), false);
  func = llvm::Function::Create(FuncTy, llvm::Function::ExternalLinkage, name,
                                Module);
}

llvm::Value *Def::codegen() {
  llvm::BasicBlock *block = llvm::BasicBlock::Create(*Context, "", func);
  llvm::BasicBlock *prevPosition = Builder->GetInsertBlock();
  Builder->SetInsertPoint(block);
  scope->codegen();
  Builder->SetInsertPoint(prevPosition);
  return func;
}

void Def::dump() const {
  std::cout << "Function " << name << "\n";
  scope->dump();
}

// ----------------------------------------------------
// ------ Call -----------------------------------------
llvm::Value *Call::codegen() {
  INode *nodeFunction = scope->findFunc(name);
  assert(nodeFunction && "Function wasn't found!");
  Def *defFunction = cast<Def>(nodeFunction);
  llvm::Function *function = defFunction->getFunc();

  if (!args) {
    return Builder->CreateCall(function, llvm::ArrayRef<llvm::Value *>());
  }

  args->codegen();
  return Builder->CreateCall(function, args->getValues());
}
void Call::dump() const { std::cout << "Call of function: " << name << "\n"; }
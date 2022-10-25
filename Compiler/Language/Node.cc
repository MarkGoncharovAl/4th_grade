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
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Utils.h"

#include "Node.hpp"

extern llvm::LLVMContext* Context;
extern llvm::IRBuilder<>* Builder;
extern llvm::Function* mainFunction;
extern llvm::Module* Module;

INode* make_value (int v) { return new Value { v }; }
INode* make_op (INode* l , Ops o , INode* r) { return new Op { l, o, r }; }
INode* make_while (INode* o , INode* s) { return new While { o, s }; }
INode* make_if (INode* cmp , INode* scope) { return new If { cmp, scope }; }
INode* make_def (const std::string& name , INode* scope) { return new Def { name, scope }; }
IScope* create_scope () { return new Scope { nullptr }; }

llvm::Value* Value::codegen ()
{
  return llvm::ConstantInt::get (llvm::Type::getInt32Ty (*Context) , val);
}
void Value::dump () const { std::cout << "Node Value: " << val << std::endl; }

llvm::Value* Decl::codegen ()
{
  llvm::Value* V = NamedValues[varname];
  assert (V && "Variable wasn't found!");
  return Builder->CreateLoad (V , varname.c_str ());
}
llvm::Value* Decl::assign (llvm::Value* RHS)
{
  llvm::Value* V = NamedValues[varname];
  assert (V && "Variable wasn't found!");
  return Builder->CreateStore (RHS , V);
}

void Decl::dump () const { std::cout << "Node Decl: " << val << std::endl; }
void Decl::SetValue (int Val) { val = Val; }

// SCOPE
llvm::Value* Scope::codegen ()
{
  for (auto x : branches)
    x->codegen ();
  return nullptr;
}
void Scope::dump () const
{
  std::cout << "Node Scope: " << std::endl;
  for (auto const x : branches)
    x->dump ();
}
IScope* Scope::resetScope () const { return prev_scope; }
void Scope::addBranch (INode* branch) { branches.push_back (branch); }

INode* Scope::access (std::string const& var_name)
{
  INode* t = find (var_name);
  if (t)
    return t;

  INode* d = new Decl { var_name };

  llvm::AllocaInst* Alloca = Builder->CreateAlloca (llvm::Type::getInt32Ty (*Context) , 0 ,
                              var_name.c_str ());
  NamedValues[var_name] = Alloca;

  add (var_name , d);
  return d;
}

void Scope::add (std::string symbol , INode* n)
{
  symbols_[symbol] = n;
}

llvm::Value* Scope::findAlloc (const std::string& symbol)
{
  auto it = allocs_.find (symbol);
  if (it == allocs_.end ())
  {
    if (prev_scope != nullptr)
      return prev_scope->find (symbol);
    else
      return nullptr;
  }
  return it->second;
}

INode* Scope::find (const std::string& symbol)
{
  auto it = symbols_.find (symbol);
  if (it == symbols_.end ())
  {
    if (prev_scope != nullptr)
      return prev_scope->find (symbol);
    else
      return nullptr;
  }
  return it->second;
}


Scope::~Scope ()
{
  for (auto x : branches)
    delete x;
}

llvm::Value* Op::codegen ()
{
  llvm::Value* LeftV = nullptr;
  llvm::Value* RightV = nullptr;
  if (left)
    LeftV = left->codegen ();
  if (right)
    RightV = right->codegen ();
  switch (op)
  {
  case Ops::Plus:
    if (LeftV == nullptr)
      LeftV =
      llvm::ConstantInt::get (llvm::Type::getInt32Ty (*Context) , 0);
    return Builder->CreateAdd (LeftV , RightV);
  case Ops::Minus:
    if (LeftV == nullptr)
      LeftV =
      llvm::ConstantInt::get (llvm::Type::getInt32Ty (*Context) , 0);
    return Builder->CreateSub (LeftV , RightV);
  case Ops::Greater:
    return Builder->CreateICmpSGT (LeftV , RightV);
  case Ops::Less:
    return Builder->CreateICmpSLT (LeftV , RightV);
  case Ops::GreaterEq:
    return Builder->CreateICmpSGE (LeftV , RightV);
  case Ops::LessEq:
    return Builder->CreateICmpSLE (LeftV , RightV);
  case Ops::Assign:
    return static_cast<Decl*>(left)->assign (RightV);
  case Ops::StdOut: {
    auto* CalleeF = Module->getFunction ("__print_int");
    assert (CalleeF && "Driver shall create decl for __print_int");
    assert (RightV && "Print required non-null right arg");
    llvm::Value* ArgsV [] = { RightV };
    return Builder->CreateCall (CalleeF , ArgsV);
  }
  case Ops::StdIn: {
    auto* CalleeF = Module->getFunction ("__pcl_scan");
    assert (CalleeF && "Driver shall create decl for __pcl_scan");
    return Builder->CreateCall (CalleeF);
  }
  case Ops::Equal:
    return Builder->CreateICmpEQ (LeftV , RightV);
  case Ops::NotEqual:
    return Builder->CreateICmpNE (LeftV , RightV);
  case Ops::Not:
    return Builder->CreateNot (RightV);
  case Ops::And:
    return Builder->CreateAnd (LeftV , RightV);
  case Ops::Or:
    return Builder->CreateOr (LeftV , RightV);
  case Ops::Div:
    return Builder->CreateSDiv (LeftV , RightV);
  case Ops::Mul:
    return Builder->CreateMul (LeftV , RightV);
  default:
    break;
  }

  // to implement
  return nullptr;
}

void Op::dump () const
{
  std::cout << "Node Op: ";
  switch (op)
  {
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
    left->dump ();
  right->dump ();
}

Op::~Op ()
{
  if (left && typeid(Decl) != typeid(*left))
    delete left;
  if (right && typeid(Decl) != typeid(*right))
    delete right;
}


llvm::Value* While::codegen ()
{
  llvm::BasicBlock* block_pre = llvm::BasicBlock::Create (*Context , "" , mainFunction);
  llvm::BasicBlock* block_true = llvm::BasicBlock::Create (*Context , "" , mainFunction);
  llvm::BasicBlock* block_out = llvm::BasicBlock::Create (*Context , "" , mainFunction);

  Builder->CreateBr (block_pre);
  Builder->SetInsertPoint (block_pre);
  llvm::Value* ret = cmp->codegen ();
  Builder->CreateCondBr (ret , block_true , block_out);

  Builder->SetInsertPoint (block_true);
  scope->codegen ();
  Builder->CreateBr (block_pre);

  Builder->SetInsertPoint (block_out);
  return ret;
}

void While::dump () const
{
  std::cout << "Node While " << std::endl;
  scope->dump ();
}

llvm::Value* If::codegen ()
{
  llvm::BasicBlock* block_true = llvm::BasicBlock::Create (*Context , "" , mainFunction);
  llvm::BasicBlock* block_out = llvm::BasicBlock::Create (*Context , "" , mainFunction);
  llvm::Value* ret = cmp->codegen ();
  Builder->CreateCondBr (ret , block_true , block_out);
  Builder->SetInsertPoint (block_true);
  scope->codegen ();
  Builder->CreateBr (block_out);
  Builder->SetInsertPoint (block_out);
  return ret;
}

void If::dump () const
{
  std::cout << "Node If:\nCmp: ";
  cmp->dump ();
  std::cout << "\nScope: ";
  scope->dump ();
}

Def::Def (const std::string& def_name , INode* def_scope)
  : name (def_name) ,
  scope (def_scope) ,
  func (nullptr)
{
  llvm::FunctionType* FuncTy = llvm::FunctionType::get (
Builder->getInt32Ty () ,
llvm::ArrayRef<llvm::Type*>{} ,
false);
  func = llvm::Function::Create (
    FuncTy , llvm::Function::ExternalLinkage , name , Module);
  NamedValues[name] = func;
}

llvm::Value* Def::codegen ()
{
  llvm::BasicBlock* block = llvm::BasicBlock::Create (*Context , "" , func);
  llvm::BasicBlock* prevPosition = Builder->GetInsertBlock ();
  Builder->SetInsertPoint (block);
  scope->codegen ();
  Builder->CreateRetVoid ();
  Builder->SetInsertPoint (prevPosition);
  return func;
}

void Def::dump () const
{
  std::cout << "Function " << name << "\n";
  scope->dump ();
}

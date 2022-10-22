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
#include "Symtab.hpp"

Symtab globalTable;
std::map<std::string , llvm::Value*> NamedValues;

extern llvm::LLVMContext* Context;
extern llvm::IRBuilder<>* Builder;
extern llvm::Function* mainFunction;
extern llvm::Module* Module;

static llvm::AllocaInst* CreateEntryBlockAlloca (const std::string& varname)
{

  llvm::IRBuilder<> TmpB (&mainFunction->getEntryBlock () ,
                         mainFunction->getEntryBlock ().begin ());
  return TmpB.CreateAlloca (llvm::Type::getInt32Ty (*Context) , 0 ,
                           varname.c_str ());
}

INode* make_value (int v) { return new Value { v }; }
INode* make_op (INode* l , Ops o , INode* r) { return new Op { l, o, r }; }
INode* make_while (INode* o , INode* s) { return new While { o, s }; }
INode* make_if (INode* cmp , INode* scope) { return new If { cmp, scope }; }
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
  INode* t = visible (var_name);
  if (t)
    return t;

  INode* d = new Decl { var_name };

  auto* Alloca = CreateEntryBlockAlloca (var_name);
  NamedValues[var_name] = Alloca;

  globalTable.add (this , var_name , d);
  return d;
}

INode* Scope::visible (std::string const& var_name)
{
  INode* f = globalTable.exists (this , var_name);
  if (f)
    return f;
  if (prev_scope)
    f = prev_scope->visible (var_name);
  return f;
}

Scope::~Scope ()
{
  for (auto x : branches)
    delete x;

  globalTable.free (this);
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
  // to implement
  return nullptr;
}

void While::dump () const
{
  std::cout << "Node While " << std::endl;
  scope->dump ();
}
While::~While ()
{
  if (op && typeid(Decl) != typeid(*op))
    delete op;
  delete scope;
}

llvm::Value* If::codegen ()
{
  block_true = llvm::BasicBlock::Create (*Context , "" , mainFunction);
  block_out = llvm::BasicBlock::Create (*Context , "" , mainFunction);
  llvm::Value* ret = cmp->codegen ();
  Builder->CreateCondBr (ret , block_true , block_out);
  Builder->SetInsertPoint (block_true);
  Builder->CreateBr (block_out);
  Builder->SetInsertPoint (block_out);
  return ret;
}

void If::dump () const
{
  std::cout << "Node If " << std::endl;
  scope->dump ();
}

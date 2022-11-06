#pragma once

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
  Or,
  Get,
  Set
};

enum class Types {
  INode,
  IScope,
  Value,
  LLVMValue,
  Storage,
  Names,
  Call,
  Def,
  If,
  While,
  Op,
  Scope,
  Decl,
  DeclArr,
  DeclArrOp,
  Ret
};
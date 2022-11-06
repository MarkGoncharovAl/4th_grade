//------------------------------------------------------------------------------
//
// parser.hpp -- bison related extern defines and retval struct
//
//------------------------------------------------------------------------------

#pragma once

#include "Node.hpp"
#include <cstdarg>
#include <cstring>
#include <sstream>
#include <stack>
#include <string>
#include <variant>

extern IScope *currentScope;
extern std::stack<llvm::Function *> stackFunction;

#define YYERROR_VERBOSE 1

// function to run after parse
int wholeProgramAction();

struct yyRet {
  std::string name;
  std::variant<int, std::string, Ops> value;
  int linePos;
  int inLinePos;
  INode *treeNode;

  bool isInt() const noexcept { return value.index() == 0; }
  int getInt() const noexcept {
    assert(isInt() && "Can't get int from not int yyRet!");
    return std::get<int>(value);
  }
  bool isString() const noexcept { return value.index() == 1; }
  std::string getString() const noexcept {
    assert(isString() && "Can't get string from not string yyRet!");
    return std::get<std::string>(value);
  }
  bool isOp() const noexcept { return value.index() == 2; }
  Ops getOp() const noexcept {
    assert(isOp() && "Can't get operation from not op yyRet!");
    return std::get<Ops>(value);
  }

  // should be placed concept!
  template <typename T> void set(const T &data) { value.emplace<T>(data); }
};

#define YYSTYPE yyRet

#include "compiler.cpp.h"

extern FILE *yyin;

int yylex();
int yyerror(char const *);
void PrintError(char const *s);
void BeginToken(char *, int *);

// line number diagnostics
extern int yylineno;

// for string position diagnostics
static int yyinlinePos;

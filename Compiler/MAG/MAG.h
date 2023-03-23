//===-- MAG.h - Top-level interface for MAG representation ----*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM MAG back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MAG_MAG_H
#define LLVM_LIB_TARGET_MAG_MAG_H

#include "MCTargetDesc/MAGMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
  class MAGTargetMachine;
  class FunctionPass;

} // end namespace llvm;

#endif

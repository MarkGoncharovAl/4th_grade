//===-- MAGTargetInfo.cpp - MAG Target Implementation -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#include "MAG.h"
#include "llvm/IR/Module.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target llvm::TheMAGTarget;

extern "C" void LLVMInitializeMAGTargetInfo() {
  RegisterTarget<Triple::mag,
                 /*HasJIT=*/false>
      X(TheMAGTarget, "MAG", "MAG (32-bit MAGulator arch)", "MAG");
}

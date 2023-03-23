//===-- MAGMCTargetDesc.h - MAG Target Descriptions -----------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file provides MAG specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_MAG_MCTARGETDESC_MAGMCTARGETDESC_H
#define LLVM_LIB_TARGET_MAG_MCTARGETDESC_MAGMCTARGETDESC_H

#include "llvm/Support/DataTypes.h"

#include <memory>

namespace llvm {
class Target;
class Triple;

extern Target TheMAGTarget;

} // End llvm namespace

// Defines symbolic names for MAG registers.  This defines a mapping from
// register name to register number.
#define GET_REGINFO_ENUM
#include "MAGGenRegisterInfo.inc"

// Defines symbolic names for the MAG instructions.
#define GET_INSTRINFO_ENUM
#include "MAGGenInstrInfo.inc"

#endif

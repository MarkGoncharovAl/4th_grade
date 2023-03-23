//===-- MAG.td - Describe the MAG Target Machine ----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
// This is the top level entry point for the MAG target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Target-dependent interfaces
//===----------------------------------------------------------------------===//

include "MAGRegisterInfo.td"

//===----------------------------------------------------------------------===//
// Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "MAGInstrInfo.td"
def MAGInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// MAG processors supported.
//===----------------------------------------------------------------------===//

// Will generate MAGGenAsmWrite.inc included by MAGInstPrinter.cpp, contents
//  as follows,
// void MAGInstPrinter::printInstruction(const MCInst *MI, raw_ostream &O) {...}
// const char *MAGInstPrinter::getRegisterName(unsigned RegNo) {...}
def MAG : Target {
  let InstructionSet = MAGInstrInfo;
}

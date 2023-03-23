//===- MAGInstrInfo.td - Target Description for MAG Target -*- tablegen -*-=//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the MAG implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// MAG profiles and nodes
//===----------------------------------------------------------------------===//

def SDT_MAGRet          : SDTypeProfile<0, 1, [SDTCisInt<0>]>;

// Return
def MAGRet : SDNode<"MAGISD::Ret", SDTNone,
                     [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "MAGInstrFormats.td"

//===----------------------------------------------------------------------===//
// MAG Operand, Complex Patterns and Transformations Definitions.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instructions specific format
//===----------------------------------------------------------------------===//

//@Branches {
let isBranch=1, isTerminator=1, isBarrier=1, hasDelaySlot = 1,
    isIndirectBranch = 1 in
class Branches<bits<8> op, string instr_asm, RegisterClass RC>:
  MAGInst<op, (outs), (ins RC:$r1),
     !strconcat(instr_asm, "\t$r1"), [(brind RC:$r1)]> {
}
//@Branches }

// Return instruction
class RetBase<RegisterClass RC>: Branches<0x3c, "RET", RC> {
  let isReturn = 1;
  let isCodeGenOnly = 1;
  let hasCtrlDep = 1;
  let hasExtraSrcRegAllocReq = 1;
}


//===----------------------------------------------------------------------===//
// Instruction definition
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// MAG Instructions
//===----------------------------------------------------------------------===//

def RET     : RetBase<CPURegs>;

/// No operation
def NOP   : MAGInst<0, (outs), (ins), "nop", []>;

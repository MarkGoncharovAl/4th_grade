//===-- MAGRegisterInfo.td - MAG Register defs -----------*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the MAG register file
//===----------------------------------------------------------------------===//

// For tablegen(... -gen-emitter)  in CMakeLists.txt
class MAGReg<bits<16> Enc, string n> : Register<n> {
  let HWEncoding = Enc;
}

// MAG CPU Registers
class MAGGPRReg<bits<16> Enc, string n> : MAGReg<Enc, n>;

//===----------------------------------------------------------------------===//
//@Registers
//===----------------------------------------------------------------------===//
//@ All registers definition
let Namespace = "MAG" in {
  //@ General Purpose Registers
  def R0    : MAGGPRReg<0,  "r0">,    DwarfRegNum<[0]>;
  def R1    : MAGGPRReg<1,  "r1">,    DwarfRegNum<[1]>;
  def R2    : MAGGPRReg<2,  "r2">,    DwarfRegNum<[2]>;
  def R3    : MAGGPRReg<3,  "r3">,    DwarfRegNum<[3]>;
  def R4    : MAGGPRReg<4,  "r4">,    DwarfRegNum<[4]>;
  def R5    : MAGGPRReg<5,  "r5">,    DwarfRegNum<[5]>;
  def R6    : MAGGPRReg<6,  "r6">,    DwarfRegNum<[6]>;
  def R7    : MAGGPRReg<7,  "r7">,    DwarfRegNum<[7]>;
  def R8    : MAGGPRReg<8,  "r8">,    DwarfRegNum<[8]>;
  def R9    : MAGGPRReg<9,  "r9">,    DwarfRegNum<[9]>;
  def R10   : MAGGPRReg<10, "r10">,   DwarfRegNum<[10]>;
  def R11   : MAGGPRReg<11, "r11">,   DwarfRegNum<[11]>;
  def R12   : MAGGPRReg<12, "r12">,   DwarfRegNum<[12]>;
  def R13   : MAGGPRReg<13, "r13">,   DwarfRegNum<[13]>;
  def R14   : MAGGPRReg<14, "r14">,   DwarfRegNum<[14]>;
  def R15   : MAGGPRReg<15, "r15">,   DwarfRegNum<[15]>;
}

//===----------------------------------------------------------------------===//
//@Register Classes
//===----------------------------------------------------------------------===//

def CPURegs : RegisterClass<"MAG", [i32], 32, (add
  R0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15)>;

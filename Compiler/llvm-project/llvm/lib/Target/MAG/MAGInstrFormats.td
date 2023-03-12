//===-- MAGInstrFormats.td - MAG Instruction Formats -----*- tablegen -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

class InstFormat<bits<5> val> {
  bits<5> Value = val;
}

def InstFormatPseudo : InstFormat<0>;
def InstFormatR      : InstFormat<1>;
def InstFormatR4     : InstFormat<2>;
def InstFormatI      : InstFormat<3>;
def InstFormatS      : InstFormat<4>;
def InstFormatB      : InstFormat<5>;
def InstFormatU      : InstFormat<6>;
def InstFormatJ      : InstFormat<7>;

class MAGOpcode<string name, bits<7> val> {
  string Name = name;
  bits<7> Value = val;
}
def MAGOpcodesList : GenericTable {
  let FilterClass = "MAGOpcode";
  let Fields = [
    "Name", "Value"
  ];
  let PrimaryKey = [ "Value" ];
  let PrimaryKeyName = "lookupMAGOpcodeByValue";
}
def lookupMAGOpcodeByName : SearchIndex {
  let Table = MAGOpcodesList;
  let Key = [ "Name" ];
}
def OPC_LOAD      : MAGOpcode<"LOAD",      0b0000011>;
def OPC_LOAD_FP   : MAGOpcode<"LOAD_FP",   0b0000111>;
def OPC_CUSTOM_0  : MAGOpcode<"CUSTOM_0",  0b0001011>;
def OPC_MISC_MEM  : MAGOpcode<"MISC_MEM",  0b0001111>;
def OPC_OP_IMM    : MAGOpcode<"OP_IMM",    0b0010011>;
def OPC_AUIPC     : MAGOpcode<"AUIPC",     0b0010111>;
def OPC_OP_IMM_32 : MAGOpcode<"OP_IMM_32", 0b0011011>;
def OPC_STORE     : MAGOpcode<"STORE",     0b0100011>;
def OPC_STORE_FP  : MAGOpcode<"STORE_FP",  0b0100111>;
def OPC_CUSTOM_1  : MAGOpcode<"CUSTOM_1",  0b0101011>;
def OPC_AMO       : MAGOpcode<"AMO",       0b0101111>;
def OPC_OP        : MAGOpcode<"OP",        0b0110011>;
def OPC_LUI       : MAGOpcode<"LUI",       0b0110111>;
def OPC_OP_32     : MAGOpcode<"OP_32",     0b0111011>;
def OPC_MADD      : MAGOpcode<"MADD",      0b1000011>;
def OPC_MSUB      : MAGOpcode<"MSUB",      0b1000111>;
def OPC_NMSUB     : MAGOpcode<"NMSUB",     0b1001011>;
def OPC_NMADD     : MAGOpcode<"NMADD",     0b1001111>;
def OPC_OP_FP     : MAGOpcode<"OP_FP",     0b1010011>;
def OPC_OP_V      : MAGOpcode<"OP_V",      0b1010111>;
def OPC_CUSTOM_2  : MAGOpcode<"CUSTOM_2",  0b1011011>;
def OPC_BRANCH    : MAGOpcode<"BRANCH",    0b1100011>;
def OPC_JALR      : MAGOpcode<"JALR",      0b1100111>;
def OPC_JAL       : MAGOpcode<"JAL",       0b1101111>;
def OPC_SYSTEM    : MAGOpcode<"SYSTEM",    0b1110011>;
def OPC_CUSTOM_3  : MAGOpcode<"CUSTOM_3",  0b1111011>;

class RVInst<dag outs, dag ins, string opcodestr, string argstr,
             list<dag> pattern, InstFormat format>
    : Instruction {
  field bits<32> Inst;
  // SoftFail is a field the disassembler can use to provide a way for
  // instructions to not match without killing the whole decode process. It is
  // mainly used for ARM, but Tablegen expects this field to exist or it fails
  // to build the decode table.
  field bits<32> SoftFail = 0;
  let Size = 4;

  bits<7> Opcode = 0;

  let Inst{6-0} = Opcode;

  let Namespace = "MAG";

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = opcodestr # "\t" # argstr;
  let Pattern = pattern;

  let TSFlags{4-0} = format.Value;


  bits<3> VLMul = 0;
  let TSFlags{10-8} = VLMul;

  bit HasDummyMask = 0;
  let TSFlags{11} = HasDummyMask;

  bit ForceTailAgnostic = false;
  let TSFlags{12} = ForceTailAgnostic;

  bit HasMergeOp = 0;
  let TSFlags{13} = HasMergeOp;

  bit HasSEWOp = 0;
  let TSFlags{14} = HasSEWOp;

  bit HasVLOp = 0;
  let TSFlags{15} = HasVLOp;

  bit HasVecPolicyOp = 0;
  let TSFlags{16} = HasVecPolicyOp;

  bit IsRVVWideningReduction = 0;
  let TSFlags{17} = IsRVVWideningReduction;

  bit UsesMaskPolicy = 0;
  let TSFlags{18} = UsesMaskPolicy;

  // Indicates that the result can be considered sign extended from bit 31. Some
  // instructions with this flag aren't W instructions, but are either sign
  // extended from a smaller size, always outputs a small integer, or put zeros
  // in bits 63:31. Used by the SExtWRemoval pass.
  bit IsSignExtendingOpW = 0;
  let TSFlags{19} = IsSignExtendingOpW;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, list<dag> pattern, string opcodestr = "", string argstr = "">
    : RVInst<outs, ins, opcodestr, argstr, pattern, InstFormatPseudo> {
  let isPseudo = 1;
  let isCodeGenOnly = 1;
}

class PseudoQuietFCMP<RegisterClass Ty>
    : Pseudo<(outs GPR:$rd), (ins Ty:$rs1, Ty:$rs2), []> {
  let hasSideEffects = 1;
  let mayLoad = 0;
  let mayStore = 0;
}

// Pseudo load instructions.
class PseudoLoad<string opcodestr, RegisterClass rdty = GPR>
    : Pseudo<(outs rdty:$rd), (ins bare_symbol:$addr), [], opcodestr, "$rd, $addr"> {
  let hasSideEffects = 0;
  let mayLoad = 1;
  let mayStore = 0;
  let isCodeGenOnly = 0;
  let isAsmParserOnly = 1;
}

class PseudoFloatLoad<string opcodestr, RegisterClass rdty = GPR>
    : Pseudo<(outs GPR:$tmp, rdty:$rd), (ins bare_symbol:$addr), [], opcodestr, "$rd, $addr, $tmp"> {
  let hasSideEffects = 0;
  let mayLoad = 1;
  let mayStore = 0;
  let isCodeGenOnly = 0;
  let isAsmParserOnly = 1;
}

// Pseudo store instructions.
class PseudoStore<string opcodestr, RegisterClass rsty = GPR>
    : Pseudo<(outs GPR:$tmp), (ins rsty:$rs, bare_symbol:$addr), [], opcodestr, "$rs, $addr, $tmp"> {
  let hasSideEffects = 0;
  let mayLoad = 0;
  let mayStore = 1;
  let isCodeGenOnly = 0;
  let isAsmParserOnly = 1;
}

// Instruction formats are listed in the order they appear in the RISC-V
// instruction set manual (R, I, S, B, U, J) with sub-formats (e.g. RVInstR4,
// RVInstRAtomic) sorted alphabetically.

class RVInstR<bits<7> funct7, bits<3> funct3, MAGOpcode opcode, dag outs,
              dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatR> {
  bits<5> rs2;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-25} = funct7;
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}

class RVInstR4<bits<2> funct2, bits<3> funct3, MAGOpcode opcode, dag outs,
               dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatR4> {
  bits<5> rs3;
  bits<5> rs2;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-27} = rs3;
  let Inst{26-25} = funct2;
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}

class RVInstI<bits<3> funct3, MAGOpcode opcode, dag outs, dag ins,
              string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<12> imm12;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-20} = imm12;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}

class RVInstIShift<bits<5> imm11_7, bits<3> funct3, MAGOpcode opcode,
                   dag outs, dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<6> shamt;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-27} = imm11_7;
  let Inst{26} = 0;
  let Inst{25-20} = shamt;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}

class RVInstIShiftW<bits<7> imm11_5, bits<3> funct3, MAGOpcode opcode,
                    dag outs, dag ins, string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatI> {
  bits<5> shamt;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-25} = imm11_5;
  let Inst{24-20} = shamt;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}

class RVInstS<bits<3> funct3, MAGOpcode opcode, dag outs, dag ins,
              string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatS> {
  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31-25} = imm12{11-5};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = imm12{4-0};
  let Opcode = opcode.Value;
}

class RVInstB<bits<3> funct3, MAGOpcode opcode, dag outs, dag ins,
              string opcodestr, string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatB> {
  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31} = imm12{11};
  let Inst{30-25} = imm12{9-4};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-8} = imm12{3-0};
  let Inst{7} = imm12{10};
  let Opcode = opcode.Value;
}

class RVInstU<MAGOpcode opcode, dag outs, dag ins, string opcodestr,
              string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatU> {
  bits<20> imm20;
  bits<5> rd;

  let Inst{31-12} = imm20;
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}

class RVInstJ<MAGOpcode opcode, dag outs, dag ins, string opcodestr,
              string argstr>
    : RVInst<outs, ins, opcodestr, argstr, [], InstFormatJ> {
  bits<20> imm20;
  bits<5> rd;

  let Inst{31} = imm20{19};
  let Inst{30-21} = imm20{9-0};
  let Inst{20} = imm20{10};
  let Inst{19-12} = imm20{18-11};
  let Inst{11-7} = rd;
  let Opcode = opcode.Value;
}


class DirectiveInsnR<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatR> {
  bits<7> opcode;
  bits<7> funct7;
  bits<3> funct3;

  bits<5> rs2;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-25} = funct7;
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode;

  let AsmString = ".insn r " # argstr;
}

class DirectiveInsnR4<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatR4> {
  bits<7> opcode;
  bits<2> funct2;
  bits<3> funct3;

  bits<5> rs3;
  bits<5> rs2;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-27} = rs3;
  let Inst{26-25} = funct2;
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode;

  let AsmString = ".insn r4 " # argstr;
}

class DirectiveInsnI<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatI> {
  bits<7> opcode;
  bits<3> funct3;

  bits<12> imm12;
  bits<5> rs1;
  bits<5> rd;

  let Inst{31-20} = imm12;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = rd;
  let Opcode = opcode;

  let AsmString = ".insn i " # argstr;
}

class DirectiveInsnS<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatS> {
  bits<7> opcode;
  bits<3> funct3;

  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31-25} = imm12{11-5};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-7} = imm12{4-0};
  let Opcode = opcode;

  let AsmString = ".insn s " # argstr;
}

class DirectiveInsnB<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatB> {
  bits<7> opcode;
  bits<3> funct3;

  bits<12> imm12;
  bits<5> rs2;
  bits<5> rs1;

  let Inst{31} = imm12{11};
  let Inst{30-25} = imm12{9-4};
  let Inst{24-20} = rs2;
  let Inst{19-15} = rs1;
  let Inst{14-12} = funct3;
  let Inst{11-8} = imm12{3-0};
  let Inst{7} = imm12{10};
  let Opcode = opcode;

  let AsmString = ".insn b " # argstr;
}

class DirectiveInsnU<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatU> {
  bits<7> opcode;

  bits<20> imm20;
  bits<5> rd;

  let Inst{31-12} = imm20;
  let Inst{11-7} = rd;
  let Opcode = opcode;

  let AsmString = ".insn u " # argstr;
}

class DirectiveInsnJ<dag outs, dag ins, string argstr>
  : RVInst<outs, ins, "", "", [], InstFormatJ> {
  bits<7> opcode;

  bits<20> imm20;
  bits<5> rd;

  let Inst{31-12} = imm20;
  let Inst{11-7} = rd;
  let Opcode = opcode;

  let AsmString = ".insn j " # argstr;
}

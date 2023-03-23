add_llvm_component_group(MAG)

set(LLVM_TARGET_DEFINITIONS MAG.td)

# Generate MAGGenRegisterInfo.inc and MAGGenInstrInfo.inc which included by
#  your hand code C++ files.
# MAGGenRegisterInfo.inc came from MAGRegisterInfo.td, MAGGenInstrInfo.inc
#  came from MAGInstrInfo.td.
tablegen(LLVM MAGGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MAGGenInstrInfo.inc -gen-instr-info)

# MAGCommonTableGen must be defined
add_public_tablegen_target(MAGCommonTableGen)

# MAGCodeGen should match with LLVMBuild.txt MAGCodeGen
add_llvm_target(MAGCodeGen
  MAGTargetMachine.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  MAGDesc
  MAGInfo
  SelectionDAG
  Support
  Target
  GlobalISel

  ADD_TO_COMPONENT
  MAG
  )

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

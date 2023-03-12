add_llvm_component_group(MAG)

set(LLVM_TARGET_DEFINITIONS MAG.td)

# Generate SimGenRegisterInfo.inc and SimGenInstrInfo.inc which included by
#  your hand code C++ files.
# SimGenRegisterInfo.inc came from SimRegisterInfo.td, SimGenInstrInfo.inc
#  came from SimInstrInfo.td.
tablegen(LLVM MAGGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM MAGGenInstrInfo.inc -gen-instr-info)

# SimCommonTableGen must be defined
add_public_tablegen_target(MAGCommonTableGen)

# SimCodeGen should match with LLVMBuild.txt MAGCodeGen
add_llvm_target(MAGCodeGen
  MAGTargetMachine.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  MC
  SimDesc
  SimInfo
  SelectionDAG
  Support
  Target
  GlobalISel

  ADD_TO_COMPONENT
  MAG
  )

add_subdirectory(TargetInfo)
add_subdirectory(MCTargetDesc)

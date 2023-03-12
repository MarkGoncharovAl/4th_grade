add_llvm_component_library(LLVMMAGDesc
  MAGMCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  MAGInfo
  Support

  ADD_TO_COMPONENT
  MAG
  )

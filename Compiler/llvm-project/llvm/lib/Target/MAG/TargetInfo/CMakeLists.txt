add_llvm_component_library(LLVMMAGInfo
  MAGTargetInfo.cpp

  LINK_COMPONENTS
  Support

  ADD_TO_COMPONENT
  MAG
  )

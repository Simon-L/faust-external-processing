install(
    TARGETS faust-base-cpp_exe
    RUNTIME COMPONENT faust-base-cpp_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()

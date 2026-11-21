cmake_minimum_required(VERSION 3.20)
project(OMTReceiver LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED FFGL_ROOT OR FFGL_ROOT STREQUAL "")
  message(FATAL_ERROR "FFGL_ROOT must point to a Resolume FFGL SDK checkout")
endif()
if(NOT DEFINED LIBOMT_ROOT OR LIBOMT_ROOT STREQUAL "")
  message(FATAL_ERROR "LIBOMT_ROOT must point to the libomt package")
endif()

file(GLOB FFGL_LIB_SOURCES
  "${FFGL_ROOT}/source/lib/ffgl/*.cpp"
  "${FFGL_ROOT}/source/lib/ffglex/*.cpp"
)

add_library(libomt SHARED IMPORTED)
if(WIN32)
  set_target_properties(libomt PROPERTIES
    IMPORTED_LOCATION "${LIBOMT_ROOT}/bin/libomt.dll"
    IMPORTED_IMPLIB "${LIBOMT_ROOT}/lib/libomt.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${LIBOMT_ROOT}/include")
else()
  message(FATAL_ERROR "This initial build configuration targets Windows x64 only.")
endif()

add_library(OMTReceiver SHARED
  source/plugins/OMTReceive/OMTReceive.cpp
  source/plugins/OMTReceive/OMTReceive.h
  source/plugins/OMTReceive/HoldingImage.h
  ${FFGL_LIB_SOURCES}
)
set_target_properties(OMTReceiver PROPERTIES PREFIX "" OUTPUT_NAME "OMTReceiver" SUFFIX ".dll")

target_include_directories(OMTReceiver PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/source/shared"
  "${FFGL_ROOT}/source/lib"
  "${FFGL_ROOT}/source/lib/ffglex"
  "${FFGL_ROOT}/deps/glew-2.1.0/include"
)
target_compile_definitions(OMTReceiver PRIVATE GLEW_STATIC)
target_link_libraries(OMTReceiver PRIVATE
  libomt
  "${FFGL_ROOT}/deps/glew-2.1.0/lib/Release/x64/glew32s.lib"
  opengl32.lib
)

add_custom_command(TARGET OMTReceiver POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${LIBOMT_ROOT}/bin/libomt.dll"
    "${LIBOMT_ROOT}/bin/libvmx.dll"
    "${LIBOMT_ROOT}/bin/libomtnet.dll"
    "$<TARGET_FILE_DIR:OMTReceiver>"
  COMMENT "Copying OMT runtime DLLs"
)

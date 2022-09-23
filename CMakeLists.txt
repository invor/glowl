cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(glowl
  VERSION 0.4.0
  LANGUAGES CXX)

# Options
set(GLOWL_OPENGL_INCLUDE "NONE" CACHE STRING "Choose OpenGL include.")
set_property(CACHE GLOWL_OPENGL_INCLUDE PROPERTY STRINGS "NONE" "GLAD" "GLAD2" "GL3W" "GLEW")
option(GLOWL_USE_ARB_BINDLESS_TEXTURE "Enable bindless texture functions." ON)
set(GLOWL_USE_GLM "AUTO" CACHE STRING "Enable glm functions.")
set_property(CACHE GLOWL_USE_GLM PROPERTY STRINGS "AUTO" "ON" "OFF")
option(GLOWL_USE_NV_MESH_SHADER "Enable mesh shader defines." OFF)

# The library
add_library(glowl INTERFACE)
add_library(glowl::glowl ALIAS glowl)

target_include_directories(glowl INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if (NOT GLOWL_OPENGL_INCLUDE STREQUAL "NONE")
  target_compile_definitions(glowl INTERFACE "GLOWL_OPENGL_INCLUDE_${GLOWL_OPENGL_INCLUDE}")
endif ()

if (NOT GLOWL_USE_ARB_BINDLESS_TEXTURE)
  target_compile_definitions(glowl INTERFACE "GLOWL_NO_ARB_BINDLESS_TEXTURE")
endif ()

string(TOUPPER "${GLOWL_USE_GLM}" GLOWL_USE_GLM)
if ("${GLOWL_USE_GLM}" STREQUAL ON)
  target_compile_definitions(glowl INTERFACE "GLOWL_USE_GLM=1")
elseif ("${GLOWL_USE_GLM}" STREQUAL OFF)
  target_compile_definitions(glowl INTERFACE "GLOWL_USE_GLM=0")
endif ()

if (GLOWL_USE_NV_MESH_SHADER)
  target_compile_definitions(glowl INTERFACE "GLOWL_USE_NV_MESH_SHADER")
endif ()

# Install
include(GNUInstallDirs)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS glowl
  EXPORT glowlTargets)

install(EXPORT glowlTargets
  FILE glowlConfig.cmake
  NAMESPACE glowl::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glowl)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/glowlConfigVersion.cmake
  VERSION ${PACKAGE_VERSION}
  COMPATIBILITY SameMinorVersion)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/glowlConfigVersion.cmake
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/glowl)

export(TARGETS glowl NAMESPACE glowl:: FILE glowlConfig.cmake)

# Show files in Visual Studio.
if (MSVC)
  # Find files.
  file(GLOB_RECURSE header_files RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

  # Generate new fake target with underscore suffix to show files.
  add_custom_target(${PROJECT_NAME}_ SOURCES ${header_files})

  # Enable folders and set fake target as default project.
  set_property(GLOBAL PROPERTY USE_FOLDERS ON)
  set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME}_)
endif ()

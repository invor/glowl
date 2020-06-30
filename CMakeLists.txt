cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(glowl VERSION 0.4.0 LANGUAGES CXX)

add_library(glowl INTERFACE)
add_library(glowl::glowl ALIAS glowl)

target_include_directories(glowl
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

include(GNUInstallDirs)

install(DIRECTORY include/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS glowl EXPORT glowlTargets)

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

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

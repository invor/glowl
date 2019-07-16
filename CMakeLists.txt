cmake_minimum_required(VERSION 3.10)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
project(glowl)

file(GLOB source_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "src/*.cpp")
file(GLOB header_files RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" "include/glowl/*.hpp")

message(STATUS "SF: ${source_files}")
message(STATUS "HF: ${header_files}")

add_library(${PROJECT_NAME} STATIC ${header_files} ${source_files})

target_include_directories(${PROJECT_NAME} PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/glowl>)
                                                
install(FILES ${header_files} DESTINATION "include")

if(WIN32)
  install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION "bin")
  install(TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION "lib")
else()
  install(TARGETS ${PROJECT_NAME} DESTINATION "lib" EXPORT ${PROJECT_NAME}-target)
endif()
cmake_minimum_required(VERSION 3.0.0)

project(Image_Proc_Demo VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS -static)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/BmpReader)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SOURCE_LIST)
add_executable(Image_Proc_Demo ${SOURCE_LIST})
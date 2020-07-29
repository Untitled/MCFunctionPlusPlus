cmake_minimum_required (VERSION 3.15)

project (MCFunctionPlusPlus)
set (CMAKE_CXX_STANDARD 17)
aux_source_directory (src/ src)
add_executable (mpp ${src})

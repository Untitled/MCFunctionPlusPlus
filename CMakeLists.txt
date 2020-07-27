cmake_minimum_required (VERSION 3.15)

project (MCFunctionPlusPlus)
set (CXX_STANDARD 14)
aux_source_directory (src/ src)
add_executable (mpp ${src})

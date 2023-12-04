cmake_minimum_required (VERSION 3.5)

project(Rame)

set (CMAKE_CXX_STANDARD 20)
if (MSVC)
    add_compile_options(/W4 /WX)
    SET(INTEL "C:/Program Files (x86)/Intel/oneAPI")
else()
    add_compile_options(-Wall)
    find_package(TBB REQUIRED) 
    link_libraries(TBB::tbb)
    SET(INTEL "/opt/intel/oneapi")
endif()

SET(INTEL_COMPILER_LIB "${INTEL}/compiler/latest/windows/compiler/lib/intel64")
SET(MKL_DIR "${INTEL}/mkl/latest/lib/cmake/mkl")


add_executable (Projet-VAL "Projet-VAL.cpp" "Projet-VAL.h" "Rame.cpp" "Rame.h" "Station.h" "Station.cpp" "Superviseur.h" "Superviseur.cpp"   "SFML.cpp")

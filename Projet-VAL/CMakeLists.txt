cmake_minimum_required (VERSION 3.5)

project(Rame)

set (CMAKE_CXX_STANDARD 20)
if(MSVC)
  set(SFML_DIR "C:/Program Files/SFML/lib/cmake/SFML")
  set(SFML_STATIC_LIBRARIES TRUE)
  add_compile_options(
    $<$<CONFIG:>:/MT> #---------|
    $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
    $<$<CONFIG:Release>:/MT> #--|
  )
endif()

find_package(SFML COMPONENTS window system graphics REQUIRED)
link_libraries(sfml-graphics sfml-window)

add_executable (Projet-VAL "Projet-VAL.cpp" "Projet-VAL.h" "Rame.cpp" "Rame.h" "Station.h" "Station.cpp" "Superviseur.h" "Superviseur.cpp" )

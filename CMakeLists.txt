cmake_minimum_required(VERSION 2.8)
set(cmake_module_path ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

if(${CMAKE_VERSION} VERSION_LESS "2.8.12")
  set(cmake_cxx_flags "${CMAKE_CXX_FLAGS} -std=c++11")
else(${CMAKE_VERSION} VERSION_LESS "2.8.12")
  if(${CMAKE_VERSION} VERSION_LESS "3.1")
    add_compile_options(-std=c++11) # cmake 2.8.12 to 3.1
  endif(${CMAKE_VERSION} VERSION_LESS "3.1")
endif(${CMAKE_VERSION} VERSION_LESS "2.8.12")

if(NOT MSVC)
  set(cmake_cxx_flags "${CMAKE_CXX_FLAGS} -wall -pedantic -wno-vla")
endif(NOT MSVC)

find_package(Boost 1.63.0)
if(boost_found)
endif()

find_package(pybind11 REQUIRED)

pybind11_add_module(heappy SHARED src/heap.cc)

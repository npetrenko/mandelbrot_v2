cmake_minimum_required(VERSION 3.13)
project(mandelbrot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

execute_process(
  COMMAND "python3" -c "import numpy; print(numpy.get_include(), end='')"
  OUTPUT_VARIABLE NUMPY_INCLUDE_DIRS
  RESULT_VARIABLE NUMPY_NOTFOUND)

find_package(PythonLibs 3 REQUIRED)
find_package(GTest REQUIRED)

if (NUMPY_NOTFOUND)
  message(STATUS "Numpy not found")
  exit()
endif()

set(BASE_cpp cpp/image.cpp cpp/gmp_complex.cpp)

add_library(mandelbrot SHARED mandelbrot.cpp "${BASE_cpp}")

target_link_libraries(mandelbrot gmp gmpxx)

target_include_directories(mandelbrot PUBLIC ${NUMPY_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} . )

add_executable(test_build_image cpp/tests/test.cpp "${BASE_cpp}")
target_include_directories(test_build_image PUBLIC ${GTEST_INCLUDE_DIRS} .)
target_link_libraries(test_build_image gtest gtest_main gmp gmpxx)

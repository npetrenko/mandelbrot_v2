cmake_minimum_required(VERSION 3.13)
project(mandelbrot)

execute_process(
  COMMAND "python3" -c "import numpy; print(numpy.get_include(), end='')"
  OUTPUT_VARIABLE NUMPY_INCLUDE_DIRS
  RESULT_VARIABLE NUMPY_NOTFOUND)

find_package(PythonLibs 3 REQUIRED)

if (NUMPY_NOTFOUND)
  message(STATUS "Numpy not found")
  exit()
endif()

add_library(mandelbrot SHARED mandelbrot.cpp cpp/image.cpp cpp/gmp_complex.cpp)

target_link_libraries(mandelbrot gmp gmpxx)

target_include_directories(mandelbrot PUBLIC ${NUMPY_INCLUDE_DIRS} ${PYTHON_INCLUDE_DIRS} .)

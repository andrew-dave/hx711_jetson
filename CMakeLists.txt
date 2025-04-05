cmake_minimum_required(VERSION 3.10)
project(HX711_GPIOD)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GPIOD REQUIRED libgpiod)

include_directories(${GPIOD_INCLUDE_DIRS})
link_directories(${GPIOD_LIBRARY_DIRS})

add_executable(hx711 main.cpp)
target_link_libraries(hx711 ${GPIOD_LIBRARIES})

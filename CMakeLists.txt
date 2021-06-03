cmake_minimum_required(VERSION 3.10)
file(GLOB SOURCES
header/*.h
src/*.cpp)
set(CMAKE_CXX_FLAGS="-Who-psabi")
project( DisplayImage )
set(THEADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package( OpenCV REQUIRED )
include_directories( ${OpenCV_INCLUDE_DIRS} )
find_library(WIRINGPI_LIBRARIES NAMES wiringPi)
add_executable( DisplayImage main.cpp ${SOURCES})
target_link_libraries(DisplayImage ${OpenCV_LIBS} ${WIRINGPI_LIBRARIES} Threads::Threads)
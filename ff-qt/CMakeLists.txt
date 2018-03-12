cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra -pedantic")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
link_directories(../.lib)

add_executable(ff-qt WIN32 main.cpp)
target_include_directories(ff-qt PRIVATE ../include)
target_link_libraries(ff-qt Qt5::Widgets ff-core)

cmake_minimum_required(VERSION 3.5)
project(Client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
	client.cpp)

add_executable(Client ${SOURCE_FILES})
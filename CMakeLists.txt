set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.25)
project(SnorlaxServer)

add_executable(SnorlaxServer main.cpp parser.cpp parser.h response_builder.cpp response_builder.h common.h network.cpp network.h)

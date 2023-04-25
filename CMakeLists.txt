set(CMAKE_CXX_STANDARD 17)
cmake_minimum_required(VERSION 3.25)
project(SnorlaxServer)

ADD_LIBRARY(ServerLib
        src/parser.cpp
        src/network.cpp
        src/response_builder.cpp
)

add_executable(SnorlaxServer main.cpp)
target_link_libraries(SnorlaxServer ServerLib)

cmake_minimum_required(VERSION 3.14)
project(MetricsCollector)

set(CMAKE_CXX_STANDARD 20) # cтавлю 20, т.к. стандрат не упоминался
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "main.cpp"
)

add_executable(metrics_collector ${SOURCES})

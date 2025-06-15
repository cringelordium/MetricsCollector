cmake_minimum_required(VERSION 3.14)
project(MetricsCollector)

set(CMAKE_CXX_STANDARD 20) # cтавлю 20, т.к. стандрат не упоминался
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_SANITIZER "Enable sanitizer" OFF)

if(ENABLE_SANITIZER)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
    message(STATUS "ThreadSanitizer enabled")
endif()

include_directories(include)

file(GLOB_RECURSE LIB_SOURCES
    "src/*.cpp"
)

# Create the library
add_library(MetricsLib STATIC ${LIB_SOURCES})

# Create the main executable
add_executable(metrics_collector main.cpp)
target_link_libraries(metrics_collector MetricsLib)

enable_testing()
add_subdirectory(tests)
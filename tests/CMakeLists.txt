include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)
FetchContent_MakeAvailable(googletest)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB TEST_SOURCES "*.cpp")

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests MetricsLib gtest gtest_main pthread)

add_test(NAME RunAllTests COMMAND run_tests)

add_executable(ConcurrencyTest concurrency_test.cpp)
target_link_libraries(ConcurrencyTest MetricsLib gtest gtest_main pthread)

add_executable(PerformanceTest perfomance_test.cpp)
target_link_libraries(PerformanceTest MetricsLib gtest gtest_main pthread)

add_test(NAME ConcurrencyTest COMMAND ConcurrencyTest)
add_test(NAME PerformanceTest COMMAND PerformanceTest)

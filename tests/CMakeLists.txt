include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

FetchContent_Declare(
  benchmark
  URL https://github.com/google/benchmark/archive/v1.8.3.zip
)

FetchContent_MakeAvailable(googletest benchmark)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
include_directories(${benchmark_SOURCE_DIR}/include)

file(GLOB TEST_SOURCES "*.cpp")
list(FILTER TEST_SOURCES EXCLUDE REGEX ".*benchmark_test.cpp$")

add_executable(run_tests ${TEST_SOURCES})
target_link_libraries(run_tests MetricsLib gtest gtest_main pthread)

add_test(NAME RunAllTests COMMAND run_tests)

add_executable(ConcurrencyTest concurrency_test.cpp)
target_link_libraries(ConcurrencyTest MetricsLib gtest gtest_main pthread)

add_executable(PerformanceTest perfomance_test.cpp)
target_link_libraries(PerformanceTest MetricsLib gtest gtest_main pthread)

# ыыыыыыыыыыыыыыыы
add_executable(ConcurrencyTSanTest concurrency_test.cpp)
target_compile_options(ConcurrencyTSanTest PRIVATE -fsanitize=thread -g)
target_link_options(ConcurrencyTSanTest PRIVATE -fsanitize=thread)
target_link_libraries(ConcurrencyTSanTest MetricsLib gtest gtest_main pthread)

add_executable(BenchmarkTest benchmark_test.cpp)
target_link_libraries(BenchmarkTest MetricsLib benchmark::benchmark pthread)

add_test(NAME ConcurrencyTest COMMAND ConcurrencyTest)
add_test(NAME PerformanceTest COMMAND PerformanceTest)
add_test(NAME ConcurrencyTSanTest COMMAND ConcurrencyTSanTest)
add_test(NAME BenchmarkTest COMMAND BenchmarkTest)

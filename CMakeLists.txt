cmake_minimum_required(VERSION 3.0)

enable_language(CXX)
project(ArrMultithread)

# Gtests
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

SET (CMAKE_CXX_COMPILER "/usr/bin/clang++-12")
add_compile_options(-Wall -Wextra -pedantic -Werror -fno-exceptions -std=c++17)

if(THREAD_SAN_ENABLE)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

if(UB_SAN_ENABLE)
    # Thread and UB sanitizers can't work together
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
endif()

# Benchmarks run
add_executable(test test.cpp)
target_link_libraries(test ${GTEST_LIBRARIES} pthread)

add_custom_target(run_test
    COMMAND ${CMAKE_BINARY_DIR}/test
    DEPENDS test
)

# Benchmarks run
add_executable(benchmarks benchmarks.cpp)
target_link_libraries(benchmarks  pthread)

add_custom_target(run_benchmarks
    COMMAND ${CMAKE_BINARY_DIR}/benchmarks
    DEPENDS benchmarks
)

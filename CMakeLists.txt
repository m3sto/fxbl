cmake_minimum_required(VERSION 3.16)
project(FXBL VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(BUILD_TESTS "Build test suite" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(ENABLE_AVX2 "Enable AVX2 optimizations" OFF)
option(ENABLE_AVX512 "Enable AVX-512 optimizations" OFF)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -DDEBUG")

# Optional SIMD optimizations
if(ENABLE_AVX2)
    add_compile_definitions(__AVX2__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
endif()

if(ENABLE_AVX512)
    add_compile_definitions(__AVX512F__)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx512f")
endif()

# Find required packages
find_package(Threads REQUIRED)

# Main library
add_library(fxbl STATIC
    src/core/protocol.cpp
    src/lattice/ring_lwe.cpp
)

target_include_directories(fxbl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(fxbl PUBLIC
    Threads::Threads
)

# Header-only template implementations
target_include_directories(fxbl PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Executable
add_executable(fxbl-bin src/main.cpp)
target_link_libraries(fxbl-bin PRIVATE fxbl)

# Examples
if(BUILD_EXAMPLES)
    add_executable(example_basic examples/basic_usage.cpp)
    target_link_libraries(example_basic PRIVATE fxbl)
    
    add_executable(example_homomorphic examples/homomorphic_ops.cpp)
    target_link_libraries(example_homomorphic PRIVATE fxbl)
    
    add_executable(example_cloud examples/cloud_computing.cpp)
    target_link_libraries(example_cloud PRIVATE fxbl)
    
    # Test executables
    add_executable(test_file_encryption examples/test_file_encryption.cpp)
    target_link_libraries(test_file_encryption PRIVATE fxbl)
    
    add_executable(test_security_attacks examples/test_security_attacks.cpp)
    target_link_libraries(test_security_attacks PRIVATE fxbl)
    
    add_executable(test_homomorphic_simple examples/test_homomorphic_simple.cpp)
    target_link_libraries(test_homomorphic_simple PRIVATE fxbl)
endif()

# Tests (placeholder - disabled until implemented)
if(BUILD_TESTS)
    enable_testing()
    
    # TODO: Add test files when implemented
    # add_executable(test_neurocrypt
    #     tests/test_core.cpp
    #     tests/test_lattice.cpp
    #     tests/test_homomorphic.cpp
    #     tests/test_security.cpp
    # )
    # target_link_libraries(test_neurocrypt PRIVATE neurocrypt)
    # add_test(NAME NeuroCryptTests COMMAND test_neurocrypt)
endif()

# Benchmarks (placeholder - disabled until implemented)
if(BUILD_BENCHMARKS)
    # TODO: Add benchmark files when implemented
    # add_executable(bench_neurocrypt benchmarks/benchmark_main.cpp)
    # target_link_libraries(bench_neurocrypt PRIVATE neurocrypt)
endif()

# Installation
install(TARGETS fxbl fxbl-bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/ DESTINATION include)

# CPack for packaging
set(CPACK_PACKAGE_NAME "FXBL")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Next-Gen Cryptographic Protocol with FHE and Post-Quantum Security")
include(CPack)

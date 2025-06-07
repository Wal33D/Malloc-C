cmake_minimum_required(VERSION 3.10)
project(nu_malloc C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)

add_library(nu_malloc_static STATIC src/nu_malloc.c)
set_target_properties(nu_malloc_static PROPERTIES
    OUTPUT_NAME nu_malloc
    POSITION_INDEPENDENT_CODE ON)
target_include_directories(nu_malloc_static PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_library(nu_malloc_shared SHARED src/nu_malloc.c)
set_target_properties(nu_malloc_shared PROPERTIES
    OUTPUT_NAME nu_malloc
    POSITION_INDEPENDENT_CODE ON)
target_include_directories(nu_malloc_shared PUBLIC ${PROJECT_SOURCE_DIR}/include)

add_executable(example src/example.c)
target_include_directories(example PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(example PRIVATE nu_malloc_static)

add_executable(memory_test tests/memory_test.c)
target_include_directories(memory_test PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(memory_test PRIVATE nu_malloc_static)

enable_testing()
add_test(NAME example_test
    COMMAND ${PROJECT_SOURCE_DIR}/tests/test_example.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/example)
add_test(NAME memory_test
    COMMAND ${PROJECT_SOURCE_DIR}/tests/test_memory.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/memory_test)

install(TARGETS nu_malloc_static nu_malloc_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib)
install(FILES include/nu_malloc.h DESTINATION include)


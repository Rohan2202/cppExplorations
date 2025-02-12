set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -O2")

set(TARGET_NAME mutexVsAtomic)
add_executable(${TARGET_NAME} src/mutex_vs_atomic.cpp)
target_precompile_headers(${TARGET_NAME} PRIVATE <iostream> <chrono>)

set(TARGET2_NAME templateVsVirtual)
add_executable(${TARGET2_NAME} src/templateVsVirtual.cpp)
target_precompile_headers(${TARGET2_NAME} PRIVATE <iostream> <chrono>)

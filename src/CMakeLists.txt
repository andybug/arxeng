# --- compiler settings ---

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

add_definitions(-g -Wall -Wextra -pedantic -D_POSIX_C_SOURCE=200809L)
include_directories(${CMAKE_SOURCE_DIR}/include)


# --- build ---

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

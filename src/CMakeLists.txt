# --- compiler settings ---

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c11")

add_definitions(-Wall -Wextra -pedantic)
include_directories(${CMAKE_SOURCE_DIR}/include)


# --- build ---

add_subdirectory(common)
add_subdirectory(server)
add_subdirectory(client)

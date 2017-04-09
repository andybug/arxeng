add_executable(
  arxeng-server
  server.c
)

target_link_libraries(
  arxeng-server
  arxeng-common
  zlog
)

install(
  TARGETS arxeng-server
  RUNTIME DESTINATION bin
)

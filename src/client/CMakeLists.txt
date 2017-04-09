add_executable(
  arxeng-client
  client.c
)

target_link_libraries(
  arxeng-client
  arxeng-common
  zlog
)

install(
  TARGETS arxeng-client
  RUNTIME DESTINATION bin
)

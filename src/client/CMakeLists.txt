add_executable(
  arxeng-client
  client.c
  net.c
)

target_link_libraries(
  arxeng-client
  arxeng-common
  msgpackc
  pthread
  zlog
  zmq
)

install(
  TARGETS arxeng-client
  RUNTIME DESTINATION bin
)

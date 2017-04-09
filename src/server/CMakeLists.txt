add_executable(
  arxeng-server
  net.c
  server.c
)

target_link_libraries(
  arxeng-server
  arxeng-common
  msgpackc
  pthread
  zlog
  zmq
)

install(
  TARGETS arxeng-server
  RUNTIME DESTINATION bin
)

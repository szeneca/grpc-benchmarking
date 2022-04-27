cmake_minimum_required(VERSION 3.16)
project(grpcBenchmark)

include(common.cmake)
include_directories("/home/m/.local/include")

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(server
    server.cpp
)
add_executable(client
    client.cpp
)

add_dependencies(server hw_grpc_proto)
add_dependencies(client hw_grpc_proto)

target_link_libraries(server hw_grpc_proto)
target_link_libraries(client hw_grpc_proto)

# Proto file
get_filename_component(hw_proto "grpc/benchmark.proto" ABSOLUTE)
get_filename_component(hw_proto_path "${hw_proto}" PATH)

# Generated sources
set(hw_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/grpc/benchmark.pb.cc")
set(hw_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/grpc/benchmark.pb.h")
set(hw_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/grpc/benchmark.grpc.pb.cc")
set(hw_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/grpc/benchmark.grpc.pb.h")
add_custom_command(
      OUTPUT "${hw_proto_srcs}" "${hw_proto_hdrs}" "${hw_grpc_srcs}" "${hw_grpc_hdrs}"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}/grpc"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}/grpc"
        -I "${hw_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${hw_proto}"
      DEPENDS "${hw_proto}")

# Include generated *.pb.h files
include_directories("${CMAKE_CURRENT_BINARY_DIR}/grpc")

# hw_grpc_proto
add_library(hw_grpc_proto
  ${hw_grpc_srcs}
  ${hw_grpc_hdrs}
  ${hw_proto_srcs}
  ${hw_proto_hdrs})

  target_link_libraries(hw_grpc_proto
  ${_REFLECTION}
  ${_GRPC_GRPCPP}
  ${_PROTOBUF_LIBPROTOBUF})
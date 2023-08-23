cmake_minimum_required(VERSION 3.1)
project(guance-cpp-demo)

set(CMAKE_CXX_STANDARD 14)
add_library(foo_library foo_library/foo_library.cc)
find_package(opentelemetry-cpp CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(CURL REQUIRED)

if(WITH_OTLP_GRPC)
  add_executable(guance-cpp-otel-grpc-exporter-demo grpc_main.cc)

  target_include_directories(guance-cpp-otel-grpc-exporter-demo PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
  target_link_libraries(guance-cpp-otel-grpc-exporter-demo PRIVATE ${OPENTELEMETRY_CPP_LIBRARIES} foo_library)

  endif()

if(WITH_OTLP_HTTP)
  add_executable(guance-cpp-otel-http-exporter-demo http_main.cc)

  target_include_directories(guance-cpp-otel-http-exporter-demo PRIVATE ${OPENTELEMETRY_CPP_INCLUDE_DIRS})
  target_link_libraries(guance-cpp-otel-http-exporter-demo PRIVATE ${OPENTELEMETRY_CPP_LIBRARIES} foo_library)
  endif ()
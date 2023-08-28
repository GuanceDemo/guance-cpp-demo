#!/bin/bash

while true
do
  /guance-cpp-demo/build/guance-cpp-otel-grpc-exporter-demo ${OTEL_GRPC_EXPORTER}
  /guance-cpp-demo/build/guance-cpp-otel-http-exporter-demo ${OTEL_HTTP_EXPORTER}
  sleep 1
done

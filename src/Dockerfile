FROM pubrepo.guance.com/demo/otel-cpp-base:latest
MAINTAINER Guance

ENV workdir /guance-cpp-demo/
RUN mkdir -p ${workdir}
COPY foo_library ${workdir}/foo_library
COPY CMakeLists.txt ${workdir}/CMakeLists.txt
COPY grpc_main.cc ${workdir}/grpc_main.cc
COPY http_main.cc ${workdir}/http_main.cc
COPY start.sh ${workdir}/start.sh
RUN chmod +x ${workdir}/start.sh

WORKDIR ${workdir}
RUN mkdir build && cd build  \
    && cmake  \
      -DWITH_OTLP_GRPC=ON \
      -DWITH_OTLP_HTTP=ON \
      .. \
    && make

ENTRYPOINT ["sh", "-c", "${workdir}start.sh"]
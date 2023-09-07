# guance-cpp-demo
[观测云](https://www.guance.com/)是一款旨在解决云计算，以及云原生时代系统为每一个完整的应用构建全链路的可观测性的云服务平台。本项目通过 OpenTelemetry 接入 C++ 应用，让您快速了解接入观测云的流程和功能。

> 项目来源：[opentelemetry cpp examples](https://github.com/open-telemetry/opentelemetry-cpp/tree/main/examples/otlp)

***

## 方式一：Helm 方式安装
> 通过 Helm 安装至 Kubernetes 集群，适合快速安装 guance-cpp-demo 进行演示的场景。
### 1. 安装
```shell
git clone https://github.com/GuanceDemo/guance-cpp-demo.git
cd guance-cpp-demo/
helm upgrade -i --create-namespace guance-cpp-demo -n cpp-demo ./deployment/helm
kubectl get pod -n cpp-demo
```
### 2. 卸载
```shell
helm uninstall guance-cpp-demo -n cpp-demo
```

***

## 方式二：自动安装
> 通过 Github Action 自动打包并安装至 Kubernetes 集群，适合编译 guance-cpp-demo 源码进行演示的场景。


## 方式三：编译安装
> 本地编译代码执行，熟悉 C++ 应用通过 Opentelemetry 接入观测云。

### 1. 环境要求
选择 [Supported C++ Versions and Supported Development Platforms](https://github.com/open-telemetry/opentelemetry-cpp) 支持的 C++ 版本和开发平台。

### 2. 安装依赖包
> gRPC 和 HTTP 协议，可根据实际场景二选一

#### 2.1 通过 gRPC 协议上传数据需安装 abseil、protobuf 和 gRPC
- 安装依赖库
```shell
# ubuntu
sudo apt-get install build-essential autoconf libtool pkg-config
sudo apt-get install libgflags-dev libgtest-dev
sudo apt-get install clang libc++-dev
```

```shell
# centos
yum install -y gcc-c++ autoconf libtool
yum groupinstall -y "Development Tools"
```


- 源码编译安装

```shell
git clone -b v1.40.0 https://github.com/grpc/grpc
cd grpc/
git submodule update --init
# install protobuf
cd third_party/protobuf
./autogen.sh
./configure
make -j
make install
ldconfig
which protoc
protoc --version

# install abseil
cd ..
cd third_party/abseil-cpp/
mkdir build && cd build && cmake ..
make
make install

# install grpc
cd ../../
mkdir build && cd build && cmake ..
make
make install
```


#### 2.2 通过 HTTP 协议上传数据需安装 libcurl
```shell
wget https://curl.haxx.se/download/curl-7.81.0.tar.bz2
tar xjf curl-7.81.0.tar.bz2
cd curl-7.81.0
./configure --without-ssl
make
make install
```


### 3. 安装 opentelemetry-cpp
#### 3.1 下载 opentelemetry-cpp
```shell
git clone --recurse-submodules https://github.com/open-telemetry/opentelemetry-cpp
```

#### 3.2 编译 opentelemetry-cpp
```shell
cd opentelemetry-cpp
mkdir build
cd build
cmake -DBUILD_TESTING=OFF -DWITH_OTLP_GRPC=ON -DWITH_OTLP_HTTP=ON ..
```
> gRPC 和 HTTP 协议，可根据实际场景二选一

- DWITH_OTLP_GRPC：打开 gRPC exporter 上报数据
- DWITH_OTLP_HTTP：打开 HTTP exporter 上报数据

#### 3.3 安装 opentelemetry-cpp
```shell
cmake --build . --target all
# 安装至 /usr/local 下
cmake --install . --prefix /usr/local
```

### 4. 方式一：通过 gRPC 协议上报数据
#### 4.1 编译 demo 源码
```shell
git clone https://github.com/GuanceDemo/guance-cpp-demo.git
cd guance-cpp-demo/src/
mkdir build && cd build && cmake -DWITH_OTLP_GRPC=ON ..
make
```

#### 4.2 安装 DataKit，打开 OpenTelemetry 采集器
操作方式：[Guance OpenTelemetry](https://docs.guance.com/integrations/opentelemetry/#__tabbed_1_1)

#### 4.3 执行 demo，上报数据
```shell
# 上报地址根据实际来修改
./guance-cpp-otel-grpc-exporter-demo 127.0.0.1:4317
```

### 5. 方式二：通过 HTTP 协议上报数据
#### 5.1 编译 demo 源码
```shell
git clone https://github.com/GuanceDemo/guance-cpp-demo.git
cd guance-cpp-demo/src/
mkdir build && cd build && cmake -DWITH_OTLP_HTTP=ON ..
make
```

#### 5.2 安装 DataKit，打开 OpenTelemetry 采集器
操作方式：[Guance OpenTelemetry](https://docs.guance.com/integrations/opentelemetry/#__tabbed_1_1)

#### 5.3 执行 demo，上报数据
```shell
# 上报地址根据实际来修改
./guance-cpp-otel-http-exporter-demo http://127.0.0.1:9529/otel/v1/trace
```

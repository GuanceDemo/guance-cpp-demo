# guance-cpp-demo
[观测云](https://www.guance.com/)是一款旨在解决云计算，以及云原生时代系统为每一个完整的应用构建全链路的可观测性的云服务平台。本项目通过 OpenTelemetry 接入 C++ 应用，让您快速了解接入观测云的流程和功能。

> 项目来源：[opentelemetry cpp examples](https://github.com/open-telemetry/opentelemetry-cpp/tree/main/examples/otlp)

***

## 前提条件
[安装 DataKit Operator 和 DataKit](https://github.com/GuanceDemo/guance-datakit-demo)

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

### 1. fork 代码到自己的 Github 仓库

### 2. 创建并添加以下密钥和变量
> 创建路径：Settings --> Secrets and Variables --> Action

New repository Secret
- `KUBECONFIG`: Kubernetes config文件，用于 Action 执行机远程操作 Kubernetes 集群
- `DOCKER_USERNAME`: 镜像仓库登陆用户名
- `DOCKER_PASSWORD`: 镜像仓库登陆密码
- `GUANCE_ACCESS_TOKEN`: 观测云 Key ID，创建方式：[API Key](https://docs.guance.com/management/api-key/)

New repository Variable
- `DOCKER_REGISTRY`: 镜像仓库地址，如 pubrepo.guance.com
- `DOCKER_NAMESPACE`: 镜像仓库项目名，如 demo
- `GUANCE_REGION`: 观测云 region，可选项：[terraform-provider-guance](https://github.com/GuanceCloud/terraform-provider-guance)

> 配置后镜像地址：$DOCKER_REGISTRY/$DOCKER_NAMESPACE/demo:latest


### 3. 触发自动打包流水线
> 手动触发路径：Action --> Build and push cpp demo images --> Run Workflow

自动打包流水线会自动打包该项目镜像并上传至镜像仓库, 可通过修改 src 下任意文件或手动方式进行触发。

- `Build and push cpp demo images`: 前端 CI 流水线

### 4. 触发自动部署流水线
> 手动触发路径：Action --> Deploy cpp demo on kubernetes --> Run Workflow

自动部署流水线通过手动触发进行部署。
- `Deploy cpp demo on kubernetes`: CD 流水线

### 5. 卸载
通过手动执行以下流水线进行卸载。
- `Uninstall cpp demo on kubernetes`: 从 Kubernetes 集群删除



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

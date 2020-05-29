![C/C++ CI](https://github.com/jiejieTop/http-client/workflows/C/C++%20CI/badge.svg)
[![issues](https://img.shields.io/github/issues/jiejieTop/http-client)](https://github.com/jiejieTop/http-client/issues)
![forks](https://img.shields.io/github/forks/jiejieTop/http-client)
![stars](https://img.shields.io/github/stars/jiejieTop/http-client)
[![license](https://img.shields.io/github/license/jiejieTop/http-client)](https://github.com/jiejieTop/http-client/blob/master/LICENSE)
![](https://img.shields.io/badge/platform-Linux|Windows|Mac|Embedded-orange.svg)


```
    __     __   __                       __ _               __ 
   / /_   / /_ / /_ ____          _____ / /(♥)___   ____   / /_
  / __ \ / __// __// __ \ ______ / ___// // // _ \ / __ \ / __/
 / / / // /_ / /_ / /_/ //_____// /__ / // //  __// / / // /_  
/_/ /_/ \__/ \__// .___/        \___//_//_/ \___//_/ /_/ \__/  
                /_/                                                                   
```

# http-client

本项目是一个高稳定性与高性能的 **HTTP** 客户端，可在多平台使用，拥有极简的 **API** 接口。

整体架构如下：

![http-client架构](./png/httpclient.png)

解析一下整个架构：

- 最上方是对外的 **API** 接口。

- 接着就是 **client** 管理器，通过池化技术实现了 **client pool**，在使用的时候从池中取出 **client**，用完就放回，用户基本不需要直接接触 **client** 实例，实现了基本的 **HTTP** 方法，如 **GET/POST** 等，还实现了 **url** 解析器，解析用户传入的 **url**，生成合适的参数递交给拦截器。

- 实现工作队列，进行异步处理，在 **CPU** 支持多核的情况下，根据 **CPU** 的最大支持线程数量创建多个线程处理，将所有来自上层的请求按顺序放入工作队列，并行处理。

- **intercept** 是拦截器，比如实现 **HTTP请求**、**HTTP应答** 等，它需要依赖 **HTTP** 报文的处理。拦截器的主要的处理工作是通过上层传入的参数去设置网络信息（**host、port** 等），与服务器建立连接、重连，发起 **HTTP** 请求、解析来自服务器的响应报文，自动处理重定向及其他操作，而保证这些操作能完成的需要基础组件，通用的字符串处理、事件的回调处理、 **HTTP** 的通用信息等，最下方的 **read and write** 则是与 **network** 打交道的。

- **network** 是网络组件，它可以自动选择数据通道，如果是 **加密** 方式则通过 **tls** 进行数据传输，而 **tls** 可以选择 **mbedtls** 作为加密后端也可以选择 **openssl** 作为加密后端；也可以是 **tcp** 直连方式，最终都是通过 **tcp** 传输的，此外还实现了本地的 **routing**（或者叫 **dns** 缓存，加快域名的解析处理）。

- **platform** 是平台抽象层，封装了不同系统的东西，比如 **socke** 或者 **AT**，**线程**、**时间**、**互斥锁**、**内存管理**，这些是与系统打交道的，也是跨平台必要的封装。

- 最右边的则是通用的内容，**list** 列表的处理、**日志库**、**错误代码**、**软件随机数发生器** 等。

## 版本

| 发布版本 | 描述 | 
| --- | --- |
| [v1.0.0] | 初次发布，完成基本架构的及其稳定性验证 |

## 文档说明

欢迎各位朋友参与进来开发，所有的功能实现都有详细的文档介绍，大家可以选择 **[开发计划](./docs/plan.md)** 中的项目进行开发、探讨、提交PR，也欢迎大家提交功能需求，本项目将由 **[杰杰](https://github.com/jiejieTop)** 持续维护。

- [开发计划](./docs/plan.md)

- [平台抽象层—内存管理](./docs/platform_memory.md)

- [平台抽象层—时间管理](./docs/platform_timer.md)

- [平台抽象层—线程管理](./docs/platform_thread.md)

- [平台抽象层—互斥锁](./docs/platform_mutex.md)

- [平台抽象层—socket](./docs/platform_net_socket.md)

- [tcp 直连](./docs/nettype_tcp.md)

- [tls 加密传输](./docs/nettype_tls.md)

- [路由](./docs/routing.md)

- [network 网卡](./docs/network.md)

- [基础组件功能](./docs/utils.md)

- [通用组件功能](./docs/general.md)

- [HTTP回调事件](./docs/event.md)

- [url 解析器](./docs/url_parser.md)

- [HTTP 报文处理](./docs/message_buffer.md)

- [HTTP 请求](./docs/request.md)

- [HTTP 响应](./docs/response.md)

- [拦截器](./docs/interceptor.md)

- [工作队列](./docs/work_queue.md)

- [http-client](./docs/client.md)

- [如何提交PR？](./docs/how_to_pr.md)

## 测试代码 & 文档

- [url 解析测试](./docs/test_url_parser.md)

- [get 方法测试](./docs/test_get.md)

- [post 方法测试](./docs/test_post.md)

- [重定向测试](./docs/test_redirect.md)

- [获取文件测试](./docs/test_get_file.md)

## 参与开发

- 平台：任何 **linux** 平台，目前先暂时开发 linux 平台的，后续适配其他平台。

### 安装 cmake 及编译器

```bash
sudo apt-get install -y cmake gcc g++
```

### 编译

编写好相关的功能后进行编译并测试，**main** 函数入口在 [test/test.c](./test/test.c) 文件中。

```bash
./build.sh
```

产生的可执行文件在 **./build/bin/http-client** 路径下。

### 运行

```bash
 ./build/bin/http-client
```

观察测试结果，满足要求后提交 **PR**。

[如何提交PR？](./docs/how_to_pr.md)

## 问题

欢迎以 [GitHub Issues](https://github.com/jiejieTop/http-client/issues) 的形式提交功能需求、问题和 bug 报告等。

## 版权和许可

http-client 遵循 [Apache License v2.0](https://github.com/jiejieTop/http-client/blob/master/LICENSE) 开源协议。鼓励代码共享和尊重原作者的著作权，可以自由的使用、修改源代码，也可以将修改后的代码作为开源或闭源软件发布。

## Star & Fork

后续我还会提供更多开源项目。也非常欢迎大家设计、开发更多实用插件和功能，一起来完善 [http-client](https://github.com/jiejieTop/http-client)。如果觉得这个开源项目很赞，可以点击项目主页 右上角的 **Star** & **Fork**，同时把它推荐给更多有需要的朋友。

## 感谢参与贡献的开发者

<a href=https://github.com/murphyzhao> <img height="90" width="90" src="./png/contributors/murphyzhao.png"/></a>
<a href=https://github.com/LinTeX9527> <img height="90" width="90" src="./png/contributors/LinTeX9527.png"/></a>


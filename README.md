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

本项目是一个开发一个高稳定性与高性能的 HTTP 客户端，可在多平台使用。

整体框架如下：

![http-client架构](./png/httpclient.png)

## 文档说明

欢迎各位朋友参与进来开发，所有的功能实现都有详细的文档介绍，大家可以选择**开发计划**中的项目进行开发、探讨、提交PR，也欢迎大家提交功能需求，本项目将由 [杰杰](https://github.com/jiejieTop) 持续维护。

- [开发计划](./docs/plan.md)

- [平台抽象层—内存管理](./docs/platform_memory.md)

- [平台抽象层—时间管理](./docs/platform_timer.md)

- [平台抽象层—线程管理](./docs/platform_thread.md)

- [平台抽象层—互斥锁](./docs/platform_mutex.md)

- [平台抽象层—socket](./docs/platform_net_socket.md)

- [平台抽象层—tcp直连](./docs/platform_nettype_tcp.md)

- [平台抽象层—tls加密传输](./docs/platform_nettype_tls.md)

- [路由](./docs/routing.md)

- [network网卡](./docs/network.md)

- [基础组件功能](./docs/utils.md)

- [通用组件功能](./docs/general.md)

- [HTTP回调事件](./docs/event.md)

- [url解析器](./docs/url_parser.md)

- [HTTP报文处理](./docs/message_buffer.md)

- [HTTP请求](./docs/request.md)

- [HTTP响应](./docs/response.md)

- [拦截器](./docs/interceptor.md)

- [http-client](./docs/client.md)

- [如何提交PR？](./docs/how_to_pr.md)

## 测试代码 & 文档

- [url解析测试](./docs/test_url_parser.md)

- [get方法测试](./docs/test_get.md)

- [post方法测试](./docs/test_post.md)

- [重定向测试](./docs/test_redirect.md)

- [获取文件测试](./docs/test_get_file.md)


## 参与开发

- 平台：任何**linux**平台，目前先暂时开发linux平台的，后续适配其他平台。

### 安装cmake

```bash
sudo apt-get install -y cmake
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

观察测试结果，满足要求后提交`PR`。

[如何提交PR？](./docs/how_to_pr.md)


## 问题

欢迎以 [GitHub Issues](https://github.com/jiejieTop/http-client/issues) 的形式提交功能需求、问题和bug报告等。

## 版权和许可

http-client 遵循 [Apache License v2.0](https://github.com/jiejieTop/http-client/blob/master/LICENSE) 开源协议。鼓励代码共享和尊重原作者的著作权，可以自由的使用、修改源代码，也可以将修改后的代码作为开源或闭源软件发布。

## Star & Fork

后续我还会提供更多开源项目。也非常欢迎大家设计、开发更多实用插件和功能，一起来完善 [http-client](https://github.com/jiejieTop/http-client)。如果觉得这个开源项目很赞，可以点击项目主页 右上角的 **Star** & **Fork** ，同时把它推荐给更多有需要的朋友。

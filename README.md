# httpclient

本项目是一个力求开发一个高稳定性与高性能的 HTTP 客户端，可在多平台使用，整体框架如下：

![httpclient框架](http://qiniu.jiejie01.top/httpclient.png)

## 文档

欢迎各位朋友参与进来开发，所有相关的开发文档都有介绍，可以选择**开发计划**中的项目进行开发、探讨，本项目将持续维护。

- [开发计划](./docs/plan.md)

- [平台抽象层—内存管理](./docs/platform_memory.md)

- [平台抽象层—时间管理](./docs/platform_timer.md)

- [平台抽象层—线程管理](./docs/platform_thread.md)

- [平台抽象层—互斥锁](./docs/platform_mutex.md)

- [平台抽象层—socket](./docs/platform_net_socket.md)

- [平台抽象层—tcp直连](./docs/platform_nettype_tcp.md)

- [平台抽象层—tls加密传输](./docs/platform_nettype_tls.md)

- [network网卡](./docs/network.md)

- [基础组件功能](./docs/utils.md)

- [url解析器](./docs/url_parser.md)

- [HTTP报文处理](./docs/message_buffer.md)

- [HTTP请求](./docs/request.md)


## 参与开发

- 平台：任何**linux**平台，目前先暂时开发linux平台的，后续适配其他平台。

### 安装cmake

```bash
sudo apt-get install -y cmake
```

### 编译

编写好相关的功能后进行编译并测试，**main**函数入口在[./test/test.c](./test/test.c)。

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





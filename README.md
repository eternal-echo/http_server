# HTTP Server Demo

## 模块和组件

HTTP 服务器模块（http_server）
- 启动和管理 HTTP 服务器。
- 接受用户的 HTTP 请求。
- 解析请求中的参数（如 MAC 地址、IP 地址、端口号等）。
- 调用 WOL 库的接口，传递正确的参数。

### 项目结构

```text
.
├── CMakeLists.txt
├── app
│   └── main.cpp
├── include
│   ├── example.h
│   └── exampleConfig.h.in
├── src
│   └── example.cpp
└── tests
    ├── dummy.cpp
    └── main.cpp
```

- 源代码文件存放在 [src/](src/) 文件夹（main函数）
- 头文件存放在 [include/](include/) 文件夹
- 主程序存放在 [app/](app/) 文件夹
- 测试代码存放在 [tests/](tests/) 文件夹（默认编译为 `unit_tests`）

如果需要添加新的可执行文件，例如 `app/hello.cpp`，只需在 [CMakeLists.txt](CMakeLists.txt) 文件中添加以下两行代码：

## 构建步骤

通过创建构建目录（如 `build/`），运行 `cmake` 生成构建文件，然后使用 `make` 构建目标。

示例命令：

```bash
# WSL或ubuntu环境
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake .. -DCMAKE_BUILD_TYPE=[Debug | Coverage | Release]
make
./main
make test      # 编译并运行测试代码
```

## 测试
```bash
curl "http://localhost:8080?mac=00:11:22:33:44:55&ip=192.168.1.100&port=9"
```
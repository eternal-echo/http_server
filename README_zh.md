## 翻译

---

### 项目状态徽章

- [![项目状态：活跃 – 该项目已达到稳定、可用状态，并正在积极开发中。](http://www.repostatus.org/badges/latest/active.svg)](http://www.repostatus.org/#active)
- [![构建状态](https://travis-ci.org/bsamseth/cpp-project.svg?branch=master)](https://travis-ci.org/bsamseth/cpp-project)
- [![构建状态](https://ci.appveyor.com/api/projects/status/g9bh9kjl6ocvsvse/branch/master?svg=true)](https://ci.appveyor.com/project/bsamseth/cpp-project/branch/master)
- [![覆盖率状态](https://coveralls.io/repos/github/bsamseth/cpp-project/badge.svg?branch=master)](https://coveralls.io/github/bsamseth/cpp-project?branch=master)
- [![代码覆盖率](https://codecov.io/gh/bsamseth/cpp-project/branch/master/graph/badge.svg)](https://codecov.io/gh/bsamseth/cpp-project)
- [![Codacy 评分](https://api.codacy.com/project/badge/Grade/eb004322b0d146239a57eb242078e179)](https://www.codacy.com/app/bsamseth/cpp-project?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=bsamseth/cpp-project&amp;utm_campaign=Badge_Grade)
- [![语言等级：C/C++](https://img.shields.io/lgtm/grade/cpp/g/bsamseth/cpp-project.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/bsamseth/cpp-project/context:cpp)
- [![警报总数](https://img.shields.io/lgtm/alerts/g/bsamseth/cpp-project.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/bsamseth/cpp-project/alerts/)
- [![许可证](https://img.shields.io/badge/license-Unlicense-blue.svg)](https://github.com/bsamseth/cpp-project/blob/master/LICENSE)
- [![代码行数](https://tokei.rs/b1/github/bsamseth/cpp-project)](https://github.com/Aaronepower/tokei)
- [![解决问题的平均时间](http://isitmaintained.com/badge/resolution/bsamseth/cpp-project.svg)](http://isitmaintained.com/project/bsamseth/cpp-project "解决问题的平均时间")
- [![仍未解决的问题百分比](http://isitmaintained.com/badge/open/bsamseth/cpp-project.svg)](http://isitmaintained.com/project/bsamseth/cpp-project "仍未解决的问题百分比")

---

### C++ 项目样板代码

这是一个 C++ 项目样板代码库，您将获得以下功能：

- 将源码、头文件和主程序分开放置在不同文件夹中
- 使用现代 [CMake](https://cmake.org/) 简化编译过程
- 设置测试框架 [doctest](https://github.com/onqtam/doctest)
- 支持 [Travis-CI](https://travis-ci.org/)、[Appveyor](https://www.appveyor.com) 和 [GitHub Actions](https://github.com/features/actions) 的持续集成，兼容 C++17
- 提供代码覆盖率报告，并自动上传到 [Coveralls.io](https://coveralls.io/) 和/或 [Codecov.io](https://codecov.io)
- 使用 [Doxygen](http://www.stack.nl/~dimitri/doxygen/) 生成代码文档

![使用演示](https://i.imgur.com/foymVfy.gif)

---

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

```cmake
add_executable(main app/main.cpp)   # 指定可执行文件名称及文件路径
target_link_libraries(main PRIVATE ${LIBRARY_NAME})  # 将可执行文件链接到库文件
```

示例代码参见 [app/main.cpp](app/main.cpp)，具体构建方式详见 [CMakeLists.txt](CMakeLists.txt)。

如果新添加的可执行文件无需链接 [src/](src/) 文件夹中的库，仅需添加第一行代码。

---

### 构建步骤

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
make coverage  # 生成代码覆盖率报告
make doc       # 生成 HTML 格式的文档
```

---

### .gitignore 文件

项目中的 [.gitignore](.gitignore) 文件基于 [GitHub 的 C++ .gitignore 模板](https://github.com/github/gitignore/blob/master/C%2B%2B.gitignore)，并增加了对 `build/` 文件夹的忽略。

---

### 集成服务

- 如果该代码库已激活 [Travis-CI](https://travis-ci.org/)，在每次提交时会自动编译并运行单元测试。
- 如果该代码库已激活 [Appveyor](https://www.appveyor.com)，同样会执行上述操作。
- 如果激活了 [Coveralls](https://coveralls.io) 或 [Codecov](https://codecov.io)，代码覆盖率也会在 Travis 上部署时自动计算并上传。

---

### 初始化项目

#### 使用 GitHub 模板

点击 `Use this template` 按钮，以该模板为基础创建新代码库。

#### 从命令行初始化

如果需要创建一个新的项目，可以使用 [setup.sh](setup.sh) 脚本来清除历史记录并初始化新项目：

```bash
git clone https://github.com/bsamseth/cpp-project  # 或使用 ssh 地址
cd cpp-project
bash setup.sh
```

运行后将生成一个包含所有样板代码的全新 Git 仓库，并带有初始提交记录。
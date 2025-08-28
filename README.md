# Virex Language

Virex 是一种简洁、易学的编程语言，专注于简化编程体验，让开发者能够专注于解决问题而不是语法细节。

## 目录结构

```
.
├── ast/                    # AST 节点定义和实现
│   ├── ast.h              # AST 节点结构定义和函数声明
│   └── ast.c              # AST 节点创建和管理函数实现
├── parser/                # 解析器相关文件
│   ├── lexer.l            # Flex 词法分析器定义
│   ├── parser.y           # Bison 语法分析器定义
│   ├── parser.h           # 解析器头文件
│   ├── lex.yy.c           # Flex 生成的词法分析器代码
│   └── parser.tab.c       # Bison 生成的语法分析器代码
├── compiler/              # 编译器实现
│   ├── compiler.h         # 编译器头文件
│   └── compiler.c         # 编译器实现
├── lib/                   # 运行时库
│   ├── vcore.hpp          # 核心运行时功能
│   └── vtypes.hpp         # 类型定义
├── main.c                 # 主程序入口
├── Makefile               # 构建脚本
└── test.vix               # 测试文件
```

## 语言特性

Virex 语言支持以下特性:

1. **简洁的语法**
   ```virex
   name = "world"  // 声明变量
   a = 10          // 赋值语句
   print "Hello, " + name  // 打印语句，分号可选
   ```

2. **自动类型推导**
   - 整数: `123`
   - 浮点数: `2.5`
   - 字符串: `"hello"` 或 `'world'`

3. **数学运算**
   - 基本运算: `+`, `-`, `*`, `/`, `%`
   - 幂运算: `**`
   - 支持运算优先级和括号

4. **可选的分号**
   Virex 语言中的分号是可选的，你可以选择使用或不使用分号来结束语句

## 构建说明

### 环境要求

- GCC 编译器
- Flex (词法分析器生成器)
- Bison (语法分析器生成器)
- Make 构建工具
- g++ 编译器 (用于将生成的 C++ 代码编译成可执行文件)

在 Ubuntu/Debian 上安装依赖:
```bash
sudo apt-get update
sudo apt-get install flex bison gcc make g++
```

在 Windows 上建议使用 MSYS2:
```bash
pacman -S flex bison gcc make mingw-w64-x86_64-gcc
```

### 构建步骤

```bash
# 克隆或下载项目后进入项目目录
make          # 构建项目
make clean    # 清理构建产物
make rebuild  # 重新构建
```

## 使用方法

构建完成后，使用以下命令运行 Virex 代码文件:

```bash
./virex test.vix             # 解析并显示AST和字节码
./virex test.vix -o test     # 编译为可执行文件test (会生成并自动删除test.cpp)
./virex init                 # 初始化lib文件夹并生成运行时库文件
```

### 命令行参数说明

- `./virex <input.vix>`: 解析并显示AST和字节码
- `./virex <input.vix> -o <output_name>`: 将Virex代码编译为可执行文件
  - 自动生成 `<output_name>.cpp` 文件
  - 使用 g++ 将其编译为可执行文件 `<output_name>`
  - 自动删除临时的 `.cpp` 文件
  - 编译成功时不输出额外信息，只在失败时显示错误
- `./virex init`: 初始化项目，创建lib目录并生成运行时库文件([vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp)和[vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp))

这将输出生成的 AST 结构和字节码，或者使用 `--compile` 参数将代码编译为 C++ 源文件并保存在项目根目录。

## AST 节点类型

解析器会生成以下类型的 AST 节点：

- `AST_PROGRAM`: 程序根节点
- `AST_PRINT`: 打印语句
- `AST_ASSIGN`: 赋值语句
- `AST_BINOP`: 二元操作（加、减、乘、除、取模、幂运算）
- `AST_UNARYOP`: 一元操作（正号、负号）
- `AST_NUM_INT`: 整数字面量
- `AST_NUM_FLOAT`: 浮点数字面量
- `AST_STRING`: 字符串字面量
- `AST_IDENTIFIER`: 标识符

## 开发者文档

### 词法分析器 (lexer.l)

词法分析器使用 Flex 实现，定义了以下 token：

- 关键字: `print`, `i32`, `i64`, `f32`, `f64`, `str`
- 操作符: `=`, `+`, `-`, `*`, `/`, `%`, `**`
- 分隔符: `(`, `)`, `;`
- 字面量: 整数、浮点数、字符串
- 标识符: 符合命名规则的变量名
-注:`;`可选
### 语法分析器 (parser.y)

语法分析器使用 Bison 实现，定义了完整的语法规则：

- 程序由语句列表组成
- 语句包括打印语句、赋值语句
- 表达式支持数学运算和优先级
- 支持括号改变运算优先级

### AST 结构

AST 节点定义在 [ast.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/ast.h) 中，主要包含：

1. 节点类型枚举
2. 二元操作符和一元操作符枚举
3. AST 节点结构体，使用联合体存储不同类型节点的数据
4. 节点创建函数
5. AST 打印和内存管理函数

### 编译器

编译器可以将生成的字节码编译为可执行的 C++ 代码：

1. 字节码指令被转换为等价的 C++ 代码
2. 使用 `vcore` 库处理打印和其他操作
3. 自动生成完整的 C++ 程序，包括 `main` 函数
4. 变量使用 `auto` 关键字声明，支持类型推导
5. 生成的 C++ 文件保存在项目根目录

### 扩展建议

如需扩展语言功能，可按以下步骤进行：

1. 在 `lexer.l` 中添加新的词法规则
2. 在 `parser.y` 中添加新的语法规则
3. 在 `ast.h` 中添加新的节点类型
4. 在 `ast.c` 中实现新的节点创建函数
5. 更新 `print_ast` 函数以支持新节点类型的打印
6. 更新字节码生成器以处理新的节点类型
7. 更新编译器以将新的字节码指令转换为 C++ 代码

## 许可证

详见 [LICENSE](file:///c%3A/Users/popol/Desktop/virex/LICENSE) 文件


本文档由Doubao 1.6大模型生成
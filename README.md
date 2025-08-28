# Virex语言编译器

Virex 是一个使用 Flex 和 Bison 构建的 AST 解析器，用于解析 Virex 语言代码并生成对应的 AST。

## 项目结构

```
.
├── ast/                     # AST 节点定义和实现
│   ├── ast.c
│   ├── ast.h
│   ├── bytecode.c
│   ├── bytecode.h
│   ├── type_inference.c
│   └── type_inference.h
├── compiler/                # 编译器实现
│   ├── compiler.c
│   └── compiler.h
├── lib/                     # 运行时库
│   ├── vcore.hpp
│   └── vtypes.hpp
├── parser/                  # 解析器相关文件
│   ├── lex.yy.c
│   ├── parser.h
│   ├── parser.tab.c
│   ├── parser.tab.h
│   └── parser.y
├── DEVELOPER.md             # 开发者文档
├── Makefile                 # 构建脚本
├── README.md                # 项目说明文档
└── main.c                   # 主程序入口
```

## 特性

- 解析 Virex 语言代码
- 生成抽象语法树(AST)
- 支持变量声明、赋值、打印语句、数学运算等语言特性
- 支持多种数据类型（整数、浮点数、字符串）
- 支持数学运算和优先级
- 支持注释
- 编译为原生机器码，提供接近原生C++的执行速度
- 编译时类型推导，避免运行时类型检查开销
- 字节码中间表示，简化代码生成过程
- 字符串操作优化，提供高效的字符串连接和重复操作

## 语法示例

```
// 变量声明和赋值
x = 10;
y = 3.14;
name = "Virex";

// 数学运算
result = x + y * 2;

// 打印语句
print "Hello, World!";
print x, y, name;

// 支持混合类型的打印语句
print "The value of x is", x;
print "Result calculation:", x, "+", y, "*", 2, "=", result;

// 字符串操作
greeting = "Hello" + " " + name;
pattern = "ABC" * 3;  // "ABCABCABC"

// toint 函数支持 64 位整数
str_num = "123456789012345";
int_num = toint(str_num);  // 支持大整数转换
print int_num;
```

## 构建和使用

### 构建

```bash
make
```

### 清理

```bash
make clean
```

### 重新构建

```bash
make rebuild
```

### 使用

```bash
./virex test.vix
```

### 命令行参数说明

- `./virex <input.vix>`: 解析并显示AST和字节码
- `./virex <input.vix> -o <output_name>`: 将Virex代码编译为可执行文件
  - 自动生成 `<output_name>.cpp` 文件
  - 使用 g++ 将其编译为可执行文件 `<output_name>`
  - 自动删除临时的 `.cpp` 文件
  - 编译成功时不输出额外信息，只在失败时显示错误
- `./virex <input.vix> -b <bytecode_file>`: 将字节码输出到指定文件
  - 生成纯字节码文件，不包含行号和标题
  - 如果文件名不以 `.vbtc` 结尾，会自动添加 `.vbtc` 扩展名
  - 文件保存在项目根目录
- `./virex <input.vbtc> -c <cpp_file>`: 将 .vbtc 字节码文件编译为 .cpp C++ 代码文件
  - 当输入文件是 .vbtc 文件时，使用此参数指定输出的 .cpp 文件名
  - 生成可以直接编译的 C++ 代码
  - 生成的 .cpp 文件保存在项目根目录
- `./virex <input.vix> -o <output_name> -b <bytecode_file>`: 同时编译为可执行文件和输出字节码文件
- `./virex init`: 初始化项目，创建lib目录并生成运行时库文件([vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp)和[vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp))

这将输出生成的 AST 结构和字节码，或者使用 `-o` 参数将代码编译为 C++ 源文件并保存在项目根目录。

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
- `AST_EXPRESSION_LIST`: 表达式列表（用于支持多个参数的 print 语句）

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
4. 变量使用具体类型声明，整数类型映射为 `long long`，支持完整的 64 位整数范围
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
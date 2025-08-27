# Virex AST Parser

Virex AST Parser 是一个使用 Flex 和 Bison 构建的抽象语法树(AST)解析器，用于解析 Virex 语言代码并生成对应的 AST。

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
├── main.c                 # 主程序入口
├── Makefile               # 构建脚本
└── test.vix               # 测试文件
```

## 语言特性

Virex 语言支持以下特性：

1. **变量声明和赋值**
   ```virex
   var name = "world"  // 声明变量
   a = 10              // 赋值语句
   ```

2. **数据类型**
   - 整数: `123`
   - 浮点数: `2.5`
   - 字符串: `"hello"` 或 `'world'`

3. **打印语句**
   ```virex
   print("hello")      // 打印字符串
   print(a)            // 打印变量
   print(123)          // 打印数字
   ```

4. **数学运算**
   - 加法: `a + b`
   - 减法: `a - b`
   - 乘法: `a * b`
   - 除法: `a / b`
   - 取模: `a % b`
   - 幂运算: `a ** b`

5. **注释**
   - 单行注释: `// 这是注释`
   - 多行注释: `/* 这是多行注释 */`

## 构建说明

### 环境要求

- GCC 编译器
- Flex (词法分析器生成器)
- Bison (语法分析器生成器)
- Make 构建工具

在 Ubuntu/Debian 上安装依赖:
```bash
sudo apt-get update
sudo apt-get install flex bison gcc make
```

在 Windows 上建议使用 MSYS2:
```bash
pacman -S flex bison gcc make
```

### 构建步骤

```bash
# 克隆或下载项目后进入项目目录
make          # 构建项目
make clean    # 清理构建产物
make rebuild  # 重新构建
```

## 使用方法

构建完成后，使用以下命令解析 Virex 代码文件：

```bash
./astparser test.vix
```

这将输出生成的 AST 结构。

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

- 关键字: `print`, `var`, `i32`, `i64`, `f32`, `f64`, `str`
- 操作符: `=`, `+`, `-`, `*`, `/`, `%`, `**`
- 分隔符: `(`, `)`, `;`
- 字面量: 整数、浮点数、字符串
- 标识符: 符合命名规则的变量名

### 语法分析器 (parser.y)

语法分析器使用 Bison 实现，定义了完整的语法规则：

- 程序由语句列表组成
- 语句包括打印语句、赋值语句和变量声明
- 表达式支持数学运算和优先级
- 支持括号改变运算优先级

### AST 结构

AST 节点定义在 [ast.h](file:///c%3A/Users/popol/Desktop/virex/ast/ast.h) 中，主要包含：

1. 节点类型枚举
2. 二元操作符和一元操作符枚举
3. AST 节点结构体，使用联合体存储不同类型节点的数据
4. 节点创建函数
5. AST 打印和内存管理函数

### 扩展建议

如需扩展语言功能，可按以下步骤进行：

1. 在 `lexer.l` 中添加新的词法规则
2. 在 `parser.y` 中添加新的语法规则
3. 在 `ast.h` 中添加新的节点类型
4. 在 `ast.c` 中实现新的节点创建函数
5. 更新 `print_ast` 函数以支持新节点类型的打印

## 许可证

详见 [LICENSE](file:///c%3A/Users/popol/Desktop/virex/LICENSE) 文件



本文档由doubao1.6大模型生成
# Virex 开发者文档

本文档详细介绍了 Virex 语言编译器的内部实现，包括词法分析器、语法分析器、抽象语法树(AST)、类型推导系统、字节码生成器、代码生成器和运行时库。

## 词法分析器

词法分析器使用 Flex 实现，将 Virex 语言源代码转换为 token 流。

### 主要功能

1. 识别关键字、标识符、字面量和操作符
2. 处理注释和空白字符
3. 报告词法错误

### 支持的 Token

1. 关键字: `print`, `input`, `toint`, `i32`, `i64`, `f32`, `f64`, `str`
2. 标识符: 符合命名规则的变量名
3. 字面量: 整数、浮点数、字符串
4. 操作符: `=`, `+`, `-`, `*`, `/`, `%`, `**`, `(`, `)`, `;`, `,`
5. 特殊: 注释、换行符、错误标记

### 相关文件

- [lexer.l](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/parser/lexer.l) - Flex 词法分析器定义文件

## 语法分析器

语法分析器使用 Bison 实现，将 token 流转换为抽象语法树(AST)。

### 主要功能

1. 解析 Virex 语言语法规则
2. 构建抽象语法树
3. 报告语法错误

### 语法规则

1. 程序由语句列表组成
2. 语句包括打印语句、赋值语句和输入语句
3. 表达式支持数学运算、字符串操作和函数调用
4. 支持括号改变运算优先级

### 相关文件

- [parser.y](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/parser/parser.y) - Bison 语法分析器定义文件
- [parser.tab.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/parser/parser.tab.h) - Bison 生成的头文件
- [parser.tab.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/parser/parser.tab.c) - Bison 生成的实现文件

## 抽象语法树(AST)

AST 是编译器的核心数据结构，表示程序的结构化表示。

### 节点类型

1. `AST_PROGRAM` - 程序根节点
2. `AST_PRINT` - 打印语句
3. `AST_ASSIGN` - 赋值语句
4. `AST_BINOP` - 二元操作
5. `AST_UNARYOP` - 一元操作
6. `AST_NUM_INT` - 整数字面量
7. `AST_NUM_FLOAT` - 浮点数字面量
8. `AST_STRING` - 字符串字面量
9. `AST_IDENTIFIER` - 标识符
10. `AST_TYPE_INT32` - i32 类型声明
11. `AST_TYPE_INT64` - i64 类型声明
12. `AST_TYPE_FLOAT32` - f32 类型声明
13. `AST_TYPE_FLOAT64` - f64 类型声明
14. `AST_TYPE_STRING` - str 类型声明
15. `AST_EXPRESSION_LIST` - 表达式列表
16. `AST_INPUT` - 输入语句
17. `AST_TOINT` - toint 函数调用

### AST_EXPRESSION_LIST 节点

`AST_EXPRESSION_LIST` 节点用于表示多个表达式的列表，主要用于支持 `print` 语句中的多个参数。
该节点包含一个表达式数组和表达式计数，允许打印多个不同类型的值，这些值会用空格连接后输出。

### 相关文件

- [ast.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/ast.h) - AST 节点定义和函数声明
- [ast.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/ast.c) - AST 节点创建和管理函数实现

## 类型推导系统

类型推导系统在编译时分析 AST 节点，确定每个表达式和变量的类型。

### 支持的类型

1. `TYPE_INTEGER` - 整数类型(映射到C++的long long)
2. `TYPE_FLOAT` - 浮点类型(映射到C++的float)
3. `TYPE_STRING` - 字符串类型(映射到C++的VString)

### 主要函数

- `create_type_inference_context()` - 创建类型推导上下文
- `free_type_inference_context()` - 释放类型推导上下文
- `infer_type()` - 推导 AST 节点的类型
- `get_variable_type()` - 获取变量的推导类型
- `set_variable_type()` - 设置变量的类型
- `type_to_cpp_string()` - 将推导类型转换为 C++ 类型字符串

### 相关文件

- [type_inference.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.h) - 类型推导系统头文件
- [type_inference.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.c) - 类型推导系统实现文件

## 字节码生成器

字节码生成器将 AST 转换为线性字节码指令序列，简化了代码生成过程。

### 字节码指令

1. `BC_LOAD_CONST_INT` - 加载整数常量
2. `BC_LOAD_CONST_FLOAT` - 加载浮点数常量
3. `BC_LOAD_CONST_STRING` - 加载字符串常量
4. `BC_LOAD_NAME` - 加载变量值
5. `BC_STORE_NAME` - 存储变量值
6. `BC_PRINT` - 打印值
7. `BC_INPUT` - 输入值
8. `BC_TOINT` - 转换为整数
9. `BC_BINARY_ADD` - 二元加法操作
10. `BC_BINARY_SUB` - 二元减法操作
11. `BC_BINARY_MUL` - 二元乘法操作
12. `BC_BINARY_DIV` - 二元除法操作
13. `BC_BINARY_MOD` - 二元取模操作
14. `BC_BINARY_POW` - 二元幂运算操作
15. `BC_UNARY_MINUS` - 一元负号操作
16. `BC_UNARY_PLUS` - 一元正号操作
17. `BC_BINARY_CONCAT` - 字符串连接操作
18. `BC_BINARY_REPEAT` - 字符串重复操作

### 多参数 Print 语句处理

对于包含多个参数的 `print` 语句（例如 `print "Hello", name, "!"`），字节码生成器会按照以下步骤处理：

1. 为第一个表达式生成字节码
2. 对于后续每个表达式：
   - 生成加载空格字符串的字节码指令
   - 生成连接操作将前面的结果与空格连接
   - 为当前表达式生成字节码
   - 生成连接操作将上述结果与当前表达式连接
3. 生成最终的打印指令

这种方法确保了所有参数都按照正确的顺序连接，并用空格分隔。

### 相关数据结构

``c
typedef enum {
    BC_LOAD_CONST_INT,
    BC_LOAD_CONST_FLOAT,
    BC_LOAD_CONST_STRING,
    BC_LOAD_NAME,
    BC_STORE_NAME,
    BC_PRINT,
    BC_INPUT,
    BC_TOINT,
    BC_BINARY_ADD,
    BC_BINARY_SUB,
    BC_BINARY_MUL,
    BC_BINARY_DIV,
    BC_BINARY_MOD,
    BC_BINARY_POW,
    BC_UNARY_MINUS,
    BC_UNARY_PLUS,
    BC_BINARY_CONCAT,
    BC_BINARY_REPEAT
} ByteCodeOp;

typedef struct {
    long long int_value;      // 整数值
    double float_value;       // 浮点数值
    char* string_value;       // 字符串值
    int var_index;            // 变量索引
} ByteCodeOperand;

typedef struct {
    ByteCodeOp op;              // 操作码
    ByteCodeOperand operand;    // 操作数
} ByteCode;

typedef struct {
    ByteCode* codes;            // 字节码指令数组
    int count;                  // 指令数量
    int capacity;               // 指令数组容量
} ByteCodeList;
```

### 相关文件

- [bytecode.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.h) - 字节码生成器头文件
- [bytecode.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.c) - 字节码生成器实现文件

## 代码生成器

代码生成器负责将字节码转换为可编译的 C++ 代码。

### 工作流程

1. 生成 C++ 文件头部，包括必要的头文件和命名空间声明
2. 根据类型推导结果声明变量，使用具体类型而非 auto
3. 遍历字节码指令，将每条指令转换为相应的 C++ 代码
4. 生成 main 函数的结束部分

### 类型映射

- `TYPE_INTEGER` -> `long long`
- `TYPE_FLOAT` -> `float`
- `TYPE_STRING` -> `VString`

### 临时变量管理

代码生成器使用临时变量来处理表达式计算，通过全局计数器和栈映射来管理临时变量：

```c
static int tmp_counter = 0;              // 全局单调递增的临时变量计数器
static int stack_tmp_map[MAX_STACK];     // 将逻辑栈位置映射到已生成的临时变量
```

### 混合类型操作处理

为了支持混合类型的表达式操作（例如整数与字符串相加），代码生成器统一使用 `to_vstring()` 函数将操作数转换为字符串后再进行操作。
这种方法确保了无论操作数是整数、浮点数还是字符串，都能正确地转换为字符串并进行连接操作。

### toint 函数支持

`toint` 函数现在支持完整的 64 位整数范围。在类型推导中，整数类型被映射为 C++ 的 `long long` 类型，
这确保了对大整数的完整支持。在代码生成阶段，`toint` 函数调用会被转换为 `virex_to_int` 函数调用，
该函数可以处理字符串到 64 位整数的转换。

### 相关文件

- [compiler.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/compiler/compiler.h) - 代码生成器头文件
- [compiler.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/compiler/compiler.c) - 代码生成器实现文件

## 主程序和命令行接口

主程序 ([main.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/main.c)) 负责编译器的整体流程控制和命令行参数处理。

### 支持的命令

1. `./virex <input.vix>` - 解析并显示 AST 和字节码
2. `./virex <input.vix> -o <output_name>` - 将 Virex 代码编译为可执行文件
3. `./virex <input.vix> -b <bytecode_file>` - 将字节码输出到文件
4. `./virex <input.vix> -o <output_name> -b <bytecode_file>` - 同时编译为可执行文件和输出字节码文件
5. `./virex <input.vbtc> -c <output.cpp>` - 将 .vbtc 字节码文件编译为 .cpp C++ 代码文件
6. `./virex init` - 初始化项目，创建 lib 目录并生成运行时库文件

### 参数说明

- `-o <output_name>`: 将 Virex 代码编译为可执行文件
  - 自动生成 `<output_name>.cpp` 文件
  - 使用 g++ 将其编译为可执行文件 `<output_name>`
  - 自动删除临时的 `.cpp` 文件
  - 编译成功时不输出额外信息，只在失败时显示错误
  
- `-b <bytecode_file>`: 将字节码输出到指定文件
  - 生成纯字节码文件，不包含行号和标题
  - 如果文件名不以 `.vbtc` 结尾，会自动添加 `.vbtc` 扩展名
  - 文件保存在项目根目录
  
- `-c <cpp_file>`: 将 .vbtc 字节码文件编译为 .cpp C++ 代码文件
  - 当输入文件是 .vbtc 文件时，使用此参数指定输出的 .cpp 文件名
  - 生成可以直接编译的 C++ 代码
  - 生成的 .cpp 文件保存在项目根目录

- `init` 命令功能: 自动创建 lib 目录并生成以下运行时库文件：
  - [vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp) - 核心运行时功能，包括 print 函数等
  - [vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp) - 类型定义，包括 VString 类等

## 运行时库

Virex 使用运行时库 ([vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp) 和 [vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp)) 来提供基本功能，如打印操作和数学运算。

### 主要功能

1. `print()` - 模板函数，支持打印各种类型的值
2. `add()`, `sub()`, `mul()`, `div()` - 基本数学运算模板函数

### 相关文件

- [vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp) - 核心运行时功能
- [vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp) - 类型定义

## 扩展建议

### 添加新数据类型

1. 在 [type_inference.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.h) 中添加新的类型枚举值
2. 更新 [type_inference.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.c) 中的类型推导逻辑
3. 更新 `type_to_cpp_string()` 函数以支持新类型到 C++ 类型的映射

### 添加新操作符

1. 在 [ast.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/ast.h) 中添加新的操作符枚举值
2. 在 [parser.y](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/parser/parser.y) 中添加语法规则
3. 在 [bytecode.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.h) 中添加新的字节码指令
4. 在 [bytecode.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.c) 中实现字节码生成逻辑
5. 在 [compiler.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/compiler/compiler.c) 中实现字节码到 C++ 代码的转换

### 添加新语句类型

1. 在 [ast.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/ast.h) 中添加新的 AST 节点类型
2. 在 [parser.y](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/parser/parser.y) 中添加语法规则
3. 在 [bytecode.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.c) 中实现字节码生成逻辑
4. 在 [compiler.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/compiler/compiler.c) 中实现代码生成逻辑
5. 在 [type_inference.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.c) 中实现类型推导逻辑
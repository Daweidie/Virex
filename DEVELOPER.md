# Virex 开发者文档

本文档详细介绍了 Virex 编译器的内部工作原理，包括编译器架构、类型推导系统和代码生成过程。

## 编译器架构

Virex 编译器由以下几个主要组件构成：

1. **词法分析器 (lexer.l)** - 使用 Flex 构建，负责将源代码转换为标记流
2. **语法分析器 (parser.y)** - 使用 Bison 构建，负责将标记流转换为抽象语法树 (AST)
3. **类型推导系统 ([type_inference.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.h)/[type_inference.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/type_inference.c))** - 负责分析 AST 并推导变量和表达式的类型
4. **字节码生成器 ([bytecode.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.h)/[bytecode.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/ast/bytecode.c))** - 将 AST 转换为字节码指令序列
5. **代码生成器 ([compiler.h](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/compiler/compiler.h)/[compiler.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/compiler/compiler.c))** - 将字节码转换为可编译的 C++ 代码
6. **主程序 ([main.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/main.c))** - 控制编译器流程，处理命令行参数

## 类型推导系统

类型推导系统是 Virex 编译器的核心组件之一，它能够根据表达式的值自动推导出变量的类型。

### 支持的数据类型

1. **整数类型 (TYPE_INTEGER)** - 对应 C++ 的 `int` 类型
2. **浮点类型 (TYPE_FLOAT)** - 对应 C++ 的 `double` 类型
3. **字符串类型 (TYPE_STRING)** - 对应 C++ 的 `std::string` 类型

### 类型推导规则

1. 带引号的值被识别为字符串类型
2. 不带引号且只包含数字的值被识别为整数类型
3. 包含小数点的数字被识别为浮点类型
4. 包含非数字字符的值被识别为字符串类型
5. 对于二元操作：
   - 如果任一操作数是字符串，结果也是字符串
   - 如果任一操作数是浮点型，结果也是浮点型
   - 除法操作通常返回浮点数
   - 其他情况返回整数类型

### 相关数据结构

```c
typedef enum {
    TYPE_UNKNOWN,  // 未知类型
    TYPE_INTEGER,  // 整数类型
    TYPE_FLOAT,    // 浮点类型
    TYPE_STRING    // 字符串类型
} InferredType;

typedef struct {
    char* name;          // 变量名
    InferredType type;   // 变量类型
} VariableInfo;

typedef struct {
    VariableInfo* variables;  // 变量信息数组
    int var_count;            // 变量数量
    int var_capacity;         // 变量数组容量
} TypeInferenceContext;
```

### 主要函数

- `create_type_inference_context()` - 创建类型推导上下文
- `free_type_inference_context()` - 释放类型推导上下文
- `infer_type()` - 推导 AST 节点的类型
- `get_variable_type()` - 获取变量的推导类型
- `set_variable_type()` - 设置变量的类型
- `type_to_cpp_string()` - 将推导类型转换为 C++ 类型字符串

## 字节码生成器

字节码生成器将 AST 转换为线性字节码指令序列，简化了代码生成过程。

### 字节码指令

1. `BC_LOAD_CONST_INT` - 加载整数常量
2. `BC_LOAD_CONST_FLOAT` - 加载浮点数常量
3. `BC_LOAD_CONST_STRING` - 加载字符串常量
4. `BC_LOAD_NAME` - 加载变量值
5. `BC_STORE_NAME` - 存储变量值
6. `BC_PRINT` - 打印值
7. `BC_BINARY_ADD` - 二元加法操作
8. `BC_BINARY_SUB` - 二元减法操作
9. `BC_BINARY_MUL` - 二元乘法操作
10. `BC_BINARY_DIV` - 二元除法操作
11. `BC_BINARY_MOD` - 二元取模操作
12. `BC_BINARY_POW` - 二元幂运算操作
13. `BC_UNARY_MINUS` - 一元负号操作
14. `BC_UNARY_PLUS` - 一元正号操作
15. `BC_BINARY_CONCAT` - 字符串连接操作
16. `BC_BINARY_REPEAT` - 字符串重复操作

### 相关数据结构

```c
typedef enum {
    BC_LOAD_CONST_INT,
    BC_LOAD_CONST_FLOAT,
    BC_LOAD_CONST_STRING,
    BC_LOAD_NAME,
    BC_STORE_NAME,
    BC_PRINT,
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

## 代码生成器

代码生成器负责将字节码转换为可编译的 C++ 代码。

### 工作流程

1. 生成 C++ 文件头部，包括必要的头文件和命名空间声明
2. 根据类型推导结果声明变量，使用具体类型而非 auto
3. 遍历字节码指令，将每条指令转换为相应的 C++ 代码
4. 生成 main 函数的结束部分

### 类型映射

- `TYPE_INTEGER` -> `int`
- `TYPE_FLOAT` -> `double`
- `TYPE_STRING` -> `std::string`

### 临时变量管理

代码生成器使用临时变量来处理表达式计算，通过全局计数器和栈映射来管理临时变量：

```c
static int tmp_counter = 0;              // 全局单调递增的临时变量计数器
static int stack_tmp_map[MAX_STACK];     // 将逻辑栈位置映射到已生成的临时变量
```

## 运行时库

Virex 使用运行时库 ([vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp) 和 [vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp)) 来提供基本功能，如打印操作和数学运算。

### 主要功能

1. `print()` - 模板函数，支持打印各种类型的值
2. `add()`, `sub()`, `mul()`, `div()` - 基本数学运算模板函数

## 主程序和命令行接口

主程序 ([main.c](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/main.c)) 负责编译器的整体流程控制和命令行参数处理。

### 支持的命令

1. `./virex <input.vix>` - 解析并显示 AST 和字节码
2. `./virex <input.vix> -o <output_name>` - 将 Virex 代码编译为可执行文件
3. `./virex init` - 初始化项目，创建 lib 目录并生成运行时库文件

### init 命令功能

`init` 命令会自动创建 lib 目录并生成以下运行时库文件：
- [vcore.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vcore.hpp) - 核心运行时功能，包括 print 函数等
- [vtypes.hpp](file:///c%3A/Users/popol/Desktop/Virex_ver/virex/lib/vtypes.hpp) - 类型定义，包括 VString 类等

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

本文档由Doubao1.6大模型生成
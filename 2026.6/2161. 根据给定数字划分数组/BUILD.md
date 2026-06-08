# 编译和运行说明

## 环境要求

- C++ 编译器（推荐 g++ 或 clang++）
- C++11 或更高版本

## 快速编译和运行

### 方法一：使用 g++ 直接编译

```bash
# 编译
g++ -std=c++11 -Wall -Wextra -O2 -o solution solution.cpp

# 运行
./solution

# Windows 下运行
solution.exe
```

### 方法二：使用 Makefile（如果安装了 make）

```bash
# 编译
make

# 运行
make run

# 清理生成的文件
make clean

# 编译调试版本
make debug

# 代码检查
make lint
```

## 编译选项说明

| 选项 | 说明 |
|------|------|
| `-std=c++11` | 使用 C++11 标准 |
| `-Wall` | 显示所有警告 |
| `-Wextra` | 显示额外警告 |
| `-O2` | 优化级别 2 |
| `-g` | 生成调试信息 |
| `-Werror` | 将警告视为错误 |

## 代码检查

```bash
# 使用 g++ 进行语法检查
g++ -std=c++11 -Wall -Wextra -Werror -fsyntax-only solution.cpp

# 使用 clang-tidy（如果安装了）
clang-tidy solution.cpp -- -std=c++11
```

## 常见问题

### 1. 编译错误：不支持 C++11

**问题：** 编译器版本太旧，不支持 C++11。

**解决：** 升级编译器或使用 `-std=c++0x` 代替 `-std=c++11`。

### 2. 编译错误：找不到头文件

**问题：** 缺少标准库头文件。

**解决：** 确保安装了完整的 C++ 开发环境。

### 3. 运行时错误：段错误

**问题：** 可能是数组越界或空指针。

**解决：** 使用调试版本编译并运行：
```bash
make debug
gdb ./solution_debug
```

## 性能测试

如果需要测试代码性能，可以使用以下方法：

```bash
# 编译优化版本
g++ -std=c++11 -O3 -DNDEBUG -o solution_opt solution.cpp

# 测试运行时间
time ./solution_opt
```

## 内存检查

如果安装了 Valgrind，可以检查内存泄漏：

```bash
# 编译调试版本
g++ -std=c++11 -g -o solution_debug solution.cpp

# 运行内存检查
valgrind --leak-check=full ./solution_debug
```

## 代码格式化

如果安装了 clang-format，可以格式化代码：

```bash
# 格式化代码
clang-format -i solution.cpp

# 查看格式化结果
clang-format solution.cpp
```

---

**最后更新：2026-06-08**

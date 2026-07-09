# 2161. 根据给定数字划分数组 - 编译配置

# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

# 目标文件
TARGET = solution

# 源文件
SRC = solution.cpp

# 默认目标
all: $(TARGET)

# 编译
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# 运行
run: $(TARGET)
	./$(TARGET)

# 清理
clean:
	rm -f $(TARGET) $(TARGET).exe

# 调试版本
debug: $(SRC)
	$(CXX) -std=c++11 -Wall -Wextra -g -o $(TARGET)_debug $(SRC)

# 代码检查
lint:
	$(CXX) -std=c++11 -Wall -Wextra -Werror -fsyntax-only $(SRC)

.PHONY: all run clean debug lint

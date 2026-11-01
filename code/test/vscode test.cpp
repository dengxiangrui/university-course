// test_cpp_fix.cpp
#include <iostream>

// 强制初始化 C++ 标准流
struct StreamInitializer {
    StreamInitializer() {
        // 确保 iostream 被初始化
        std::ios_base::Init init;
        std::cout << "C++ streams initialized" << std::endl;
    }
};

// 全局对象，在 main 之前初始化
StreamInitializer streamInit;

int main() {
    // 检查流状态
    if (!std::cout.good()) {
        std::cerr << "cout is not good!" << std::endl;
        return 1;
    }
    
    std::cout << "Hello World from C++!" << std::endl;
    std::cout << "fun" << std::endl;
    std::cout << "asdf" << std::endl;
    std::cout << "qwer" << std::endl;
    
    // 测试输入
    int age;
    std::cout << "请输入年龄: ";
    if (std::cin >> age) {
        std::cout << "age=" << age << std::endl;
    } else {
        std::cout << "输入失败" << std::endl;
    }
    system("pause");
    return 0;
}
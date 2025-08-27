#ifndef VCORE_HPP
#define VCORE_HPP

#include <iostream>
#include <string>

namespace vcore {

    // 模板函数，可以打印任何类型的数据
    template<typename T>
    void print(const T& value) {
        std::cout << value << std::endl;
    }

    // 重载模板函数，专门处理bool类型，以打印"true"或"false"而不是0或1
    template<>
    inline void print<bool>(const bool& value) {
        std::cout << (value ? "true" : "false") << std::endl;
    }

    // 重载模板函数，专门处理char*类型，以避免空指针问题
    template<>
    inline void print<char*>(char* const& value) {
        if (value == nullptr) {
            std::cout << "nullptr" << std::endl;
        } else {
            std::cout << value << std::endl;
        }
    }

    // 重载模板函数，专门处理const char*类型，以避免空指针问题
    template<>
    inline void print<const char*>(const char* const& value) {
        if (value == nullptr) {
            std::cout << "nullptr" << std::endl;
        } else {
            std::cout << value << std::endl;
        }
    }

    // 重载模板函数，专门处理std::string类型
    template<>
    inline void print<std::string>(const std::string& value) {
        std::cout << value << std::endl;
    }
    
    template<typename T, typename... Args>
    void print(const T& first, const Args&... args) {
        std::cout << first;
        if constexpr (sizeof...(args) > 0) {
            std::cout << " ";
            print(args...);
        } else {
            std::cout << std::endl;
        }
    }
    template<typename T>
    T add(const T& a, const T& b) {
        return a + b;
    }
    
    template<typename T>
    T sub(const T& a, const T& b) {
        return a - b;
    }
    
    template<typename T>
    T mul(const T& a, const T& b) {
        return a * b;
    }
    
    template<typename T>
    T div(const T& a, const T& b) {
        return a / b;
    }
}

#endif 
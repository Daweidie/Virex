#ifndef VCORE_HPP
#define VCORE_HPP

#include <iostream>
#include <string>

namespace vcore {
    template<typename T>
    void print(const T& value) {
        std::cout << value << std::endl;
    }
    template<>
    inline void print<bool>(const bool& value) {
        std::cout << (value ? "true" : "false") << std::endl;
    }
    template<>
    inline void print<char*>(char* const& value) {
        if (value == nullptr) {
            std::cout << "nullptr" << std::endl;
        } else {
            std::cout << value << std::endl;
        }
    }
    template<>
    inline void print<const char*>(const char* const& value) {
        if (value == nullptr) {
            std::cout << "nullptr" << std::endl;
        } else {
            std::cout << value << std::endl;
        }
    }
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
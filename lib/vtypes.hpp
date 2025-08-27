// 一个字符串类型，继承自std::string,但是实现的+和*运算符重载，此字符串可以和字符串相加或和数字相乘。
/**
 * 用法：
 * 1. 创建一个VString对象
 * 2. 使用+运算符或*运算符进行字符串的拼接或字符串的乘法
 */

#ifndef VTYPES_HPP
#define VTYPES_HPP

#include <string>
#include <iostream>
#include <cmath>
#include <stdexcept>

namespace vtypes
{
    class VString : public std::string
    {
    public:
        using std::string::string;
        VString() : std::string() {}
        VString(const std::string &str) : std::string(str) {}
        VString operator+(const VString &other) const
        {
            return VString(static_cast<const std::string &>(*this) + static_cast<const std::string &>(other));
        }
        VString operator+(const std::string &other) const
        {
            return VString(static_cast<const std::string &>(*this) + other);
        }
        VString operator*(int times) const
        {
            VString result;
            for (int i = 0; i < times; ++i)
            {
                result += *this;
            }
            return result;
        }
        VString operator*(long times) const
        {
            VString result;
            for (long i = 0; i < times; ++i)
            {
                result += *this;
            }
            return result;
        }
        VString operator*(long long times) const
        {
            VString result;
            for (long long i = 0; i < times; ++i)
            {
                result += *this;
            }
            return result;
        }
        VString operator*(float times) const
        {
            if (times < 0 || times != std::floor(times)) {
                throw std::invalid_argument("Float multiplier must be a non-negative integer value");
            }
            VString result;
            for (int i = 0; i < static_cast<int>(times); ++i)
            {
                result += *this;
            }
            return result;
        }
        VString operator*(double times) const
        {
            if (times < 0 || times != std::floor(times)) {
                throw std::invalid_argument("Double multiplier must be a non-negative integer value");
            }
            VString result;
            for (long long i = 0; i < static_cast<long long>(times); ++i)
            {
                result += *this;
            }
            return result;
        }
        VString &operator+=(const VString &other)
        {
            static_cast<std::string &>(*this) += static_cast<const std::string &>(other);
            return *this;
        }
        VString &operator*=(int times)
        {
            VString temp = *this;
            clear();
            for (int i = 0; i < times; ++i)
            {
                *this += temp;
            }
            return *this;
        }
        VString &operator*=(long times)
        {
            VString temp = *this;
            clear();
            for (long i = 0; i < times; ++i)
            {
                *this += temp;
            }
            return *this;
        }
        VString &operator*=(long long times)
        {
            VString temp = *this;
            clear();
            for (long long i = 0; i < times; ++i)
            {
                *this += temp;
            }
            return *this;
        }
        VString &operator*=(float times)
        {
            if (times < 0 || times != std::floor(times)) {
                throw std::invalid_argument("Float multiplier must be a non-negative integer value");
            }
            VString temp = *this;
            clear();
            for (int i = 0; i < static_cast<int>(times); ++i)
            {
                *this += temp;
            }
            return *this;
        }
        VString &operator*=(double times)
        {
            if (times < 0 || times != std::floor(times)) {
                throw std::invalid_argument("Double multiplier must be a non-negative integer value");
            }
            VString temp = *this;
            clear();
            for (long long i = 0; i < static_cast<long long>(times); ++i)
            {
                *this += temp;
            }
            return *this;
        }
    };
    inline VString operator+(const std::string &lhs, const VString &rhs)
    {
        return VString(lhs + static_cast<const std::string &>(rhs));
    }
    inline VString operator*(int times, const VString &str)
    {
        return str * times;
    }
    inline VString operator*(long times, const VString &str)
    {
        return str * times;
    }
    inline VString operator*(long long times, const VString &str)
    {
        return str * times;
    }
    inline VString operator*(float times, const VString &str)
    {
        return str * times;
    }
    inline VString operator*(double times, const VString &str)
    {
        return str * times;
    }
}

#endif 

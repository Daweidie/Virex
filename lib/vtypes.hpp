// VString 类型定义
// 继承自 std::string，添加了 + 和 * 运算符重载
// 支持字符串连接和重复操作
#ifndef VTYPES_HPP
#define VTYPES_HPP
#include <string>
#include <iostream>
#include <stdexcept>

namespace vtypes {

class VString : public std::string {
public:
	using std::string::string;
	VString() : std::string() {}
	VString(const std::string &s) : std::string(s) {}
	VString(const char* s) : std::string(s) {}
	VString operator+(const VString &other) const {
		return VString(static_cast<const std::string&>(*this) + static_cast<const std::string&>(other));
	}
	VString operator+(const std::string &other) const {
		return VString(static_cast<const std::string&>(*this) + other);
	}
	friend VString operator+(const std::string &lhs, const VString &rhs) {
		return VString(lhs + static_cast<const std::string&>(rhs));
	}
	VString operator*(long long times) const {
		if (times < 0) throw std::invalid_argument("Multiplier must be non-negative");
		VString result;
		result.reserve(this->size() * (size_t)times);
		for (long long i = 0; i < times; ++i) result += *this;
		return result;
	}
	friend VString operator*(long long times, const VString &s) {
		return s * times;
	}
	VString& operator+=(const VString &other) {
		static_cast<std::string&>(*this) += static_cast<const std::string&>(other);
		return *this;
	}
	VString& operator+=(const std::string &other) {
		static_cast<std::string&>(*this) += other;
		return *this;
	}
	VString& operator*=(long long times) {
		if (times < 0) throw std::invalid_argument("Multiplier must be non-negative");
		VString tmp = *this;
		this->clear();
		for (long long i = 0; i < times; ++i) *this += tmp;
		return *this;
	}
	friend std::ostream& operator<<(std::ostream &os, const VString &s) {
		os << static_cast<const std::string&>(s);
		return os;
	}
};

} 

#endif
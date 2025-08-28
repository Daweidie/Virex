#ifndef VCONVERT_HPP
#define VCONVERT_HPP

#include <string>
#include <cstdlib>
#include "vtypes.hpp"

namespace vconvert {
    inline long long to_int(const vtypes::VString& s) { 
        return std::stoll(static_cast<const std::string&>(s)); 
    }
    
    inline long long to_int(const std::string& s) { 
        return std::stoll(s); 
    }
    
    inline long long to_int(long long v) { 
        return v; 
    }
    
    inline long long to_int(int v) { 
        return v; 
    }
    
    inline long long to_int(double v) { 
        return static_cast<long long>(v); 
    }
    
    template<typename T> 
    inline long long to_int(const T& v) { 
        return static_cast<long long>(v); 
    }
    inline double to_double(const vtypes::VString& s) { 
        return std::stod(static_cast<const std::string&>(s)); 
    }
    
    inline double to_double(const std::string& s) { 
        return std::stod(s); 
    }
    
    inline double to_double(double v) { 
        return v; 
    }
    
    inline double to_double(long long v) { 
        return static_cast<double>(v); 
    }
    
    inline double to_double(int v) { 
        return static_cast<double>(v); 
    }
    
    template<typename T> 
    inline double to_double(const T& v) { 
        return static_cast<double>(v); 
    }
    inline vtypes::VString to_vstring(const vtypes::VString& s) { 
        return s; 
    }
    
    inline vtypes::VString to_vstring(const std::string& s) { 
        return vtypes::VString(s); 
    }
    
    inline vtypes::VString to_vstring(long long v) { 
        return vtypes::VString(std::to_string(v)); 
    }
    
    inline vtypes::VString to_vstring(int v) { 
        return vtypes::VString(std::to_string(v)); 
    }
    
    inline vtypes::VString to_vstring(double v) { 
        return vtypes::VString(std::to_string(v)); 
    }
    
    template<typename T> 
    inline vtypes::VString to_vstring(const T& v) { 
        return vtypes::VString(std::to_string(static_cast<long long>(v))); 
    }
}

#endif
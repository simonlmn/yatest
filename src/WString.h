#ifndef YATEST_WSTRING_H_
#define YATEST_WSTRING_H_

#include "Arduino.h"
#include <string>
#include <cstring>
#include <cstdlib>

// Mock Arduino String class using std::string internally
class String {
private:
    std::string _str;

public:
    // Constructors
    String() : _str() {}
    String(const char* cstr) : _str(cstr ? cstr : "") {}
    String(const std::string& str) : _str(str) {}
    String(const String& str) : _str(str._str) {}
    String(const __FlashStringHelper* str) : _str(reinterpret_cast<const char*>(str)) {}
    String(char c) : _str(1, c) {}
    String(unsigned char num, unsigned char base = 10) { 
        char buf[34]; 
        ultoa(num, buf, base); 
        _str = buf; 
    }
    String(int num, unsigned char base = 10) { 
        char buf[34]; 
        itoa(num, buf, base); 
        _str = buf; 
    }
    String(unsigned int num, unsigned char base = 10) { 
        char buf[34]; 
        utoa(num, buf, base); 
        _str = buf; 
    }
    String(long num, unsigned char base = 10) { 
        char buf[34]; 
        ltoa(num, buf, base); 
        _str = buf; 
    }
    String(unsigned long num, unsigned char base = 10) { 
        char buf[34]; 
        ultoa(num, buf, base); 
        _str = buf; 
    }
    String(float num, unsigned char decimalPlaces = 2) {
        char buf[33];
        dtostrf(num, (decimalPlaces + 2), decimalPlaces, buf);
        _str = buf;
    }
    String(double num, unsigned char decimalPlaces = 2) {
        char buf[33];
        dtostrf(num, (decimalPlaces + 2), decimalPlaces, buf);
        _str = buf;
    }

    // Memory management
    unsigned char reserve(unsigned int size) { 
        _str.reserve(size); 
        return 1; 
    }
    
    // Length and capacity
    unsigned int length() const { return _str.length(); }
    
    // Character access
    char charAt(unsigned int index) const { 
        return index < _str.length() ? _str[index] : '\0'; 
    }
    void setCharAt(unsigned int index, char c) {
        if (index < _str.length()) _str[index] = c;
    }
    char operator[](unsigned int index) const { return charAt(index); }
    char& operator[](unsigned int index) { return _str[index]; }
    void getBytes(unsigned char* buf, unsigned int bufsize, unsigned int index = 0) const {
        if (!buf || !bufsize) return;
        unsigned int n = bufsize - 1;
        if (n > _str.length() - index) n = _str.length() - index;
        strncpy((char*)buf, _str.c_str() + index, n);
        buf[n] = '\0';
    }
    void toCharArray(char* buf, unsigned int bufsize, unsigned int index = 0) const {
        getBytes((unsigned char*)buf, bufsize, index);
    }
    const char* c_str() const { return _str.c_str(); }
    char* begin() { return &_str[0]; }
    char* end() { return begin() + length(); }
    const char* begin() const { return _str.c_str(); }
    const char* end() const { return _str.c_str() + length(); }

    // Concatenation
    String& operator+=(const String& rhs) { _str += rhs._str; return *this; }
    String& operator+=(const char* cstr) { if (cstr) _str += cstr; return *this; }
    String& operator+=(char c) { _str += c; return *this; }
    String& operator+=(unsigned char num) { return *this += String(num); }
    String& operator+=(int num) { return *this += String(num); }
    String& operator+=(unsigned int num) { return *this += String(num); }
    String& operator+=(long num) { return *this += String(num); }
    String& operator+=(unsigned long num) { return *this += String(num); }
    String& operator+=(float num) { return *this += String(num); }
    String& operator+=(double num) { return *this += String(num); }

    friend String operator+(const String& lhs, const String& rhs) { 
        String result = lhs; 
        result += rhs; 
        return result; 
    }
    friend String operator+(const String& lhs, const char* cstr) {
        String result = lhs;
        result += cstr;
        return result;
    }
    friend String operator+(const char* cstr, const String& rhs) {
        String result(cstr);
        result += rhs;
        return result;
    }

    String& concat(const String& str) { return *this += str; }
    String& concat(const char* cstr) { return *this += cstr; }
    String& concat(char c) { return *this += c; }
    String& concat(unsigned char num) { return *this += String(num); }
    String& concat(int num) { return *this += String(num); }
    String& concat(unsigned int num) { return *this += String(num); }
    String& concat(long num) { return *this += String(num); }
    String& concat(unsigned long num) { return *this += String(num); }
    String& concat(float num) { return *this += String(num); }
    String& concat(double num) { return *this += String(num); }

    // Comparison
    int compareTo(const String& s) const { return _str.compare(s._str); }
    unsigned char equals(const String& s) const { return _str == s._str; }
    unsigned char equals(const char* cstr) const { return _str == (cstr ? cstr : ""); }
    unsigned char operator==(const String& rhs) const { return equals(rhs); }
    unsigned char operator==(const char* cstr) const { return equals(cstr); }
    unsigned char operator!=(const String& rhs) const { return !equals(rhs); }
    unsigned char operator!=(const char* cstr) const { return !equals(cstr); }
    unsigned char operator<(const String& rhs) const { return _str < rhs._str; }
    unsigned char operator>(const String& rhs) const { return _str > rhs._str; }
    unsigned char operator<=(const String& rhs) const { return _str <= rhs._str; }
    unsigned char operator>=(const String& rhs) const { return _str >= rhs._str; }
    unsigned char equalsIgnoreCase(const String& s) const {
        if (length() != s.length()) return 0;
        for (unsigned int i = 0; i < length(); i++) {
            if (::tolower(charAt(i)) != ::tolower(s.charAt(i))) return 0;
        }
        return 1;
    }
    unsigned char startsWith(const String& prefix) const {
        return _str.length() >= prefix._str.length() && 
               _str.compare(0, prefix._str.length(), prefix._str) == 0;
    }
    unsigned char startsWith(const String& prefix, unsigned int offset) const {
        return offset <= _str.length() && 
               _str.length() - offset >= prefix._str.length() &&
               _str.compare(offset, prefix._str.length(), prefix._str) == 0;
    }
    unsigned char endsWith(const String& suffix) const {
        return _str.length() >= suffix._str.length() &&
               _str.compare(_str.length() - suffix._str.length(), suffix._str.length(), suffix._str) == 0;
    }

    // Search
    int indexOf(char ch, unsigned int fromIndex = 0) const {
        size_t pos = _str.find(ch, fromIndex);
        return pos == std::string::npos ? -1 : (int)pos;
    }
    int indexOf(const String& str, unsigned int fromIndex = 0) const {
        size_t pos = _str.find(str._str, fromIndex);
        return pos == std::string::npos ? -1 : (int)pos;
    }
    int lastIndexOf(char ch) const {
        size_t pos = _str.rfind(ch);
        return pos == std::string::npos ? -1 : (int)pos;
    }
    int lastIndexOf(char ch, unsigned int fromIndex) const {
        size_t pos = _str.rfind(ch, fromIndex);
        return pos == std::string::npos ? -1 : (int)pos;
    }
    int lastIndexOf(const String& str) const {
        size_t pos = _str.rfind(str._str);
        return pos == std::string::npos ? -1 : (int)pos;
    }
    int lastIndexOf(const String& str, unsigned int fromIndex) const {
        size_t pos = _str.rfind(str._str, fromIndex);
        return pos == std::string::npos ? -1 : (int)pos;
    }

    // Substring
    String substring(unsigned int beginIndex) const {
        return String(_str.substr(beginIndex));
    }
    String substring(unsigned int beginIndex, unsigned int endIndex) const {
        if (beginIndex > endIndex) {
            unsigned int temp = beginIndex;
            beginIndex = endIndex;
            endIndex = temp;
        }
        return String(_str.substr(beginIndex, endIndex - beginIndex));
    }

    // Modification
    void replace(char find, char replace) {
        for (size_t i = 0; i < _str.length(); i++) {
            if (_str[i] == find) _str[i] = replace;
        }
    }
    void replace(const String& find, const String& replace) {
        size_t pos = 0;
        while ((pos = _str.find(find._str, pos)) != std::string::npos) {
            _str.replace(pos, find._str.length(), replace._str);
            pos += replace._str.length();
        }
    }
    void remove(unsigned int index) {
        if (index < _str.length()) _str.erase(index);
    }
    void remove(unsigned int index, unsigned int count) {
        if (index < _str.length()) _str.erase(index, count);
    }
    void toLowerCase() {
        for (size_t i = 0; i < _str.length(); i++) {
            _str[i] = (char)::tolower((unsigned char)_str[i]);
        }
    }
    void toUpperCase() {
        for (size_t i = 0; i < _str.length(); i++) {
            _str[i] = (char)::toupper((unsigned char)_str[i]);
        }
    }
    void trim() {
        size_t start = 0;
        while (start < _str.length() && isSpace(_str[start])) start++;
        size_t end = _str.length();
        while (end > start && isSpace(_str[end - 1])) end--;
        _str = _str.substr(start, end - start);
    }

    // Conversion to numbers
    long toInt() const { return atol(_str.c_str()); }
    float toFloat() const { return (float)atof(_str.c_str()); }
    double toDouble() const { return atof(_str.c_str()); }
};

#endif // YATEST_WSTRING_H_

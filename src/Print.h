#ifndef PRINT_H
#define PRINT_H

#include <cstddef>
#include <cstring>
#include <cstdio>
#include <cstdarg>

// Mock Print class for non-Arduino compilation
// Provides the interface expected by Arduino libraries

class Print {
public:
  virtual ~Print() = default;

  // Write single byte
  virtual size_t write(uint8_t b) = 0;

  // Write string
  virtual size_t write(const char* str) {
    if (!str) return 0;
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
      write((uint8_t)str[i]);
    }
    return len;
  }

  // Write buffer with size (const char*)
  virtual size_t write(const char* buffer, size_t size) {
    size_t written = 0;
    for (size_t i = 0; i < size; i++) {
      written += write((uint8_t)buffer[i]);
    }
    return written;
  }

  // Write buffer with size (const uint8_t*)
  virtual size_t write(const uint8_t* buffer, size_t size) {
    size_t written = 0;
    for (size_t i = 0; i < size; i++) {
      written += write(buffer[i]);
    }
    return written;
  }

  // Stream compatibility
  virtual int availableForWrite() {
    return -1;  // Unknown
  }

  // Print single character
  size_t print(char c) {
    return write((uint8_t)c);
  }

  // Print string
  size_t print(const char* str) {
    return write(str);
  }

  // Print flash string helper
  size_t print(const __FlashStringHelper* str) {
    if (!str) return 0;
    // For flash strings, just cast and write as regular string
    return write((const char*)str);
  }

  // Print unsigned integer
  size_t print(unsigned int n) {
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%u", n);
    return write(buffer);
  }

  // Print signed integer
  size_t print(int n) {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%d", n);
    return write(buffer);
  }

  // Print unsigned long
  size_t print(unsigned long n) {
    char buffer[11];
    snprintf(buffer, sizeof(buffer), "%lu", n);
    return write(buffer);
  }

  // Print signed long
  size_t print(long n) {
    char buffer[12];
    snprintf(buffer, sizeof(buffer), "%ld", n);
    return write(buffer);
  }

  // Print float/double
  size_t print(double d, int digits = 2) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.*f", digits, d);
    return write(buffer);
  }

  // Print with newline
  size_t println() {
    return write("\n");
  }

  size_t println(char c) {
    return print(c) + println();
  }

  size_t println(const char* str) {
    return print(str) + println();
  }

  size_t println(int n) {
    return print(n) + println();
  }

  size_t println(unsigned int n) {
    return print(n) + println();
  }

  size_t println(long n) {
    return print(n) + println();
  }

  size_t println(unsigned long n) {
    return print(n) + println();
  }

  size_t println(double d, int digits = 2) {
    return print(d, digits) + println();
  }

  // Printf-like functionality
  size_t printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[256];
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    if (len > 0) {
      return write(buffer);
    }
    return 0;
  }
};

#endif // PRINT_H

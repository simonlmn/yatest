#ifndef YATEST_ARDUINO_H_
#define YATEST_ARDUINO_H_

// Core Arduino types and macros
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>

// PROGMEM support (no-op for native compilation)
#define PROGMEM
#define PGM_P const char*
#define PSTR(s) (s)
#define FPSTR(s) reinterpret_cast<const __FlashStringHelper*>(s)
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

#define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#define pgm_read_word(addr) (*(const unsigned short *)(addr))
#define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#define pgm_read_float(addr) (*(const float *)(addr))
#define pgm_read_ptr(addr) (*(const void **)(addr))

#define strlen_P strlen
#define strcpy_P strcpy
#define strncpy_P strncpy
#define strcmp_P strcmp
#define strncmp_P strncmp
#define strchr_P strchr
#define strstr_P strstr
#define memcpy_P memcpy
#define memcmp_P memcmp
#define memchr_P memchr
#define sprintf_P sprintf
#define snprintf_P snprintf
#define vsnprintf_P vsnprintf

// Serial configuration constants
#define SERIAL_5N1 0x00
#define SERIAL_6N1 0x02
#define SERIAL_7N1 0x04
#define SERIAL_8N1 0x06

// Pin modes
#define INPUT 0x0
#define OUTPUT 0x1
#define INPUT_PULLUP 0x2

// Digital levels
#define LOW 0x0
#define HIGH 0x1

// Math constants
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

// Character classification - use standard C functions without redefining
#define isAlphaNumeric(c) (isalnum(c))
#define isAlpha(c) (isalpha(c))
#define isAscii(c) ((c) <= 0x7f)
#define isWhitespace(c) (isspace(c))
#define isControl(c) (iscntl(c))
#define isDigit(c) (isdigit(c))
#define isGraph(c) (isgraph(c))
#define isLowerCase(c) (islower(c))
#define isPrintable(c) (isprint(c))
#define isPunct(c) (ispunct(c))
#define isSpace(c) (isspace(c))
#define isUpperCase(c) (isupper(c))
#define isHexadecimalDigit(c) (isxdigit(c))
#define toAscii(c) ((c) & 0x7f)
// Don't redefine toLowerCase/toUpperCase - they conflict with C++ String methods

// Bit manipulation
#define bit(b) (1UL << (b))
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) ((bitvalue) ? bitSet(value, bit) : bitClear(value, bit))

// Math functions - DON'T define as macros, use C++ std functions to avoid conflicts
// constrain is Arduino-specific so we can define it
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define radians(deg) ((deg)*DEG_TO_RAD)
#define degrees(rad) ((rad)*RAD_TO_DEG)
#define sq(x) ((x)*(x))

// Type definitions
typedef bool boolean;
typedef uint8_t byte;
typedef uint16_t word;

// Forward declare helper for strings stored in "PROGMEM"
class __FlashStringHelper;

// Time functions
extern unsigned long _test_millis;
extern unsigned long _test_micros;
#define _test_time _test_millis

inline unsigned long millis() {
    return _test_millis;
}

inline unsigned long micros() {
    return _test_micros;
}

inline void delay(unsigned long ms) {
    _test_millis += ms;
}

inline void delayMicroseconds(unsigned int us) {
    _test_micros += us;
}

inline void yield() {
    // No-op for tests
}

// String conversion functions (implemented by stdlib)
// Don't redefine atol/atof - just use std versions

// String conversion functions (implemented by stdlib)
inline char* itoa(int value, char* result, int base) {
    if (base < 2 || base > 36) { *result = '\0'; return result; }
    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

inline char* ltoa(long value, char* result, int base) {
    if (base < 2 || base > 36) { *result = '\0'; return result; }
    char* ptr = result, *ptr1 = result, tmp_char;
    long tmp_value;
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz"[35 + (tmp_value - value * base)];
    } while (value);
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

inline char* utoa(unsigned int value, char* result, int base) {
    if (base < 2 || base > 36) { *result = '\0'; return result; }
    char* ptr = result, *ptr1 = result, tmp_char;
    unsigned int tmp_value;
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

inline char* ultoa(unsigned long value, char* result, int base) {
    if (base < 2 || base > 36) { *result = '\0'; return result; }
    char* ptr = result, *ptr1 = result, tmp_char;
    unsigned long tmp_value;
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[tmp_value - value * base];
    } while (value);
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}

inline char* dtostrf(double val, signed char width, unsigned char prec, char* sout) {
    char fmt[20];
    sprintf(fmt, "%%%d.%df", width, prec);
    sprintf(sout, fmt, val);
    return sout;
}

// Random number functions
inline long random(long howbig) {
    if (howbig == 0) return 0;
    return rand() % howbig;
}

inline long random(long howsmall, long howbig) {
    if (howsmall >= howbig) return howsmall;
    long diff = howbig - howsmall;
    return random(diff) + howsmall;
}

inline void randomSeed(unsigned long seed) {
    srand(seed);
}

// Map function
inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Interrupt functions (no-op for testing)
inline void interrupts() {}
inline void noInterrupts() {}

// Serial/RingBuffer mocks
struct RingBuffer {
    uint8_t data[128];
    size_t readPos = 0;
    size_t writePos = 0;

    size_t available() const {
        if (writePos >= readPos) {
            return writePos - readPos;
        } else {
            return sizeof(data) - (readPos - writePos);
        }
    }

    size_t availableForWrite() const {
        return sizeof(data) - available() - 1;
    }

    bool write(uint8_t byte) {
        if (availableForWrite() == 0) {
            return false;
        }
        data[writePos] = byte;
        writePos = (writePos + 1) % sizeof(data);
        return true;
    }

    size_t write(const uint8_t* buffer, size_t length) {
        size_t bytesWritten = 0;
        while (bytesWritten < length && availableForWrite() > 0) {
            data[writePos] = buffer[bytesWritten];
            writePos = (writePos + 1) % sizeof(data);
            bytesWritten++;
        }
        return bytesWritten;
    }

    int read() {
        if (available() == 0) {
            return -1;
        }
        uint8_t byte = data[readPos];
        readPos = (readPos + 1) % sizeof(data);
        return byte;
    }

    size_t readBytes(uint8_t* buffer, size_t length) {
        size_t bytesRead = 0;
        while (bytesRead < length && available() > 0) {
            int byte = read();
            if (byte == -1) {
                break;
            }
            buffer[bytesRead] = static_cast<uint8_t>(byte);
            bytesRead++;
        }
        return bytesRead;
    }

    void flush() {
        readPos = writePos;
    }
};

class SerialMock {
public:
    RingBuffer& rxBuffer;
    RingBuffer& txBuffer;
    SerialMock(RingBuffer& rx, RingBuffer& tx) : rxBuffer(rx), txBuffer(tx) {}
    int available() { return rxBuffer.available(); }
    size_t availableForWrite() { return txBuffer.availableForWrite(); }
    size_t readBytes(uint8_t* buffer, size_t length) { return rxBuffer.readBytes(buffer, length); }
    size_t write(const uint8_t* buffer, size_t length) { return txBuffer.write(buffer, length); }
    void begin(unsigned long baud, int config) {}
    void setTimeout(unsigned long timeout) {}
    void flush() { rxBuffer.flush(); }
    int read() { return rxBuffer.read(); }
};

// Provide default Serial instance expected by Arduino sketches.
static RingBuffer SerialRxBuffer {};
static RingBuffer SerialTxBuffer {};
static SerialMock Serial { SerialRxBuffer, SerialTxBuffer };

#endif // YATEST_ARDUINO_H_

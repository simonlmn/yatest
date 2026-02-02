#ifndef YATEST_STREAM_H_
#define YATEST_STREAM_H_

#include "Arduino.h"
#include "WString.h"
#include <cstddef>

// Base class for character and binary based streams
class Stream {
protected:
    unsigned long _timeout = 1000;
    unsigned long _startMillis;

    int timedRead() {
        int c;
        _startMillis = millis();
        do {
            c = read();
            if (c >= 0) return c;
        } while(millis() - _startMillis < _timeout);
        return -1;
    }

    int timedPeek() {
        int c;
        _startMillis = millis();
        do {
            c = peek();
            if (c >= 0) return c;
        } while(millis() - _startMillis < _timeout);
        return -1;
    }

    int peekNextDigit() {
        int c;
        while (1) {
            c = timedPeek();
            if (c < 0) return c;
            if (c == '-') return c;
            if (c >= '0' && c <= '9') return c;
            read();
        }
    }

public:
    virtual ~Stream() {}

    virtual int available() = 0;
    virtual int read() = 0;
    virtual int peek() = 0;

    void setTimeout(unsigned long timeout) {
        _timeout = timeout;
    }

    unsigned long getTimeout() const {
        return _timeout;
    }

    bool find(const char* target) {
        return findUntil(target, nullptr);
    }

    bool find(const char* target, size_t length) {
        return findUntil(target, length, nullptr, 0);
    }

    bool findUntil(const char* target, const char* terminator) {
        return findUntil(target, strlen(target), terminator, strlen(terminator));
    }

    bool findUntil(const char* target, size_t targetLen, const char* terminator, size_t termLen) {
        if (terminator == nullptr) {
            termLen = 0;
        }
        size_t index = 0;
        size_t termIndex = 0;
        int c;

        _startMillis = millis();
        while ((c = timedRead()) > 0) {
            if (c == target[index]) {
                if (++index >= targetLen) return true;
            } else {
                index = 0;
            }
            if (termLen > 0 && c == terminator[termIndex]) {
                if (++termIndex >= termLen) return false;
            } else {
                termIndex = 0;
            }
        }
        return false;
    }

    long parseInt() {
        return parseInt('\0');
    }

    long parseInt(char skipChar) {
        bool isNegative = false;
        long value = 0;
        int c;

        c = peekNextDigit();
        if (c < 0) return 0;

        do {
            if (c == skipChar) {
                // ignore this character
            } else if (c == '-') {
                isNegative = true;
            } else if (c >= '0' && c <= '9') {
                value = value * 10 + c - '0';
            }
            read();
            c = timedPeek();
        } while ((c >= '0' && c <= '9') || c == skipChar);

        if (isNegative) value = -value;
        return value;
    }

    float parseFloat() {
        return parseFloat('\0');
    }

    float parseFloat(char skipChar) {
        bool isNegative = false;
        bool isFraction = false;
        long value = 0;
        int c;
        float fraction = 1.0;

        c = peekNextDigit();
        if (c < 0) return 0;

        do {
            if (c == skipChar) {
                // ignore
            } else if (c == '-') {
                isNegative = true;
            } else if (c == '.') {
                isFraction = true;
            } else if (c >= '0' && c <= '9') {
                value = value * 10 + c - '0';
                if (isFraction) fraction *= 0.1;
            }
            read();
            c = timedPeek();
        } while ((c >= '0' && c <= '9') || c == '.' || c == skipChar);

        float result = value * (isFraction ? fraction : 1.0);
        return isNegative ? -result : result;
    }

    size_t readBytes(char* buffer, size_t length) {
        return readBytes((uint8_t*)buffer, length);
    }

    virtual size_t readBytes(uint8_t* buffer, size_t length) {
        size_t count = 0;
        _startMillis = millis();
        while (count < length) {
            int c = timedRead();
            if (c < 0) break;
            *buffer++ = (uint8_t)c;
            count++;
        }
        return count;
    }

    size_t readBytesUntil(char terminator, char* buffer, size_t length) {
        return readBytesUntil(terminator, (uint8_t*)buffer, length);
    }

    size_t readBytesUntil(char terminator, uint8_t* buffer, size_t length) {
        if (length < 1) return 0;
        size_t index = 0;
        _startMillis = millis();
        while (index < length) {
            int c = timedRead();
            if (c < 0 || c == terminator) break;
            *buffer++ = (uint8_t)c;
            index++;
        }
        return index;
    }

    String readString() {
        String ret;
        int c = timedRead();
        while (c >= 0) {
            ret += (char)c;
            c = timedRead();
        }
        return ret;
    }

    String readStringUntil(char terminator) {
        String ret;
        int c = timedRead();
        while (c >= 0 && c != terminator) {
            ret += (char)c;
            c = timedRead();
        }
        return ret;
    }

    // Pure virtual write methods to be implemented by derived classes
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const uint8_t* buffer, size_t size) = 0;

    size_t write(const char* str) {
        if (str == nullptr) return 0;
        return write((const uint8_t*)str, strlen(str));
    }

    size_t write(const char* buffer, size_t size) {
        return write((const uint8_t*)buffer, size);
    }

    virtual void flush() {}
};

#endif // YATEST_STREAM_H_

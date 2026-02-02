#ifndef YATEST_MOCKS_H_
#define YATEST_MOCKS_H_

// Include all Arduino mock headers
#include "../Arduino.h"
#include "../WString.h"
#include "../Stream.h"

// Additional helper functions for time advancement
inline void advanceTimeMs(unsigned long millis_delta) {
  _test_millis += millis_delta;
  _test_micros += millis_delta * 1000;
}

inline void advanceTimeUs(unsigned long micros_delta) {
  _test_micros += micros_delta;
  _test_millis += micros_delta / 1000;
}

// Legacy aliases for compatibility
#define _mock_millis _test_millis
#define _mock_micros _test_micros

#endif

#ifndef YATEST_EXPECT_H_
#define YATEST_EXPECT_H_

#include <stdexcept>

namespace yatest {

struct ExpectationFailed final : public std::logic_error {
  using logic_error::logic_error;
};

inline void expect(bool expectation, const char* what) { if (!expectation) throw ExpectationFailed(what); }

}

#endif

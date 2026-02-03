#ifndef YATEST_TESTRUNNER_H_
#define YATEST_TESTRUNNER_H_

#include "TestSuite.h"
#include <iostream>
#include <iomanip>
#include <string>

namespace yatest {

inline bool& useColorOutput() {
  static bool enabled = true;
  return enabled;
}

inline void setUseColor(bool enabled) {
  useColorOutput() = enabled;
}

inline std::string colorize(const char* code, const std::string& text) {
  if (!useColorOutput()) {
    return text;
  }
  return std::string(code) + text + "\033[0m";
}

/**
 * Run all test suites currently listed in yatest::TestSuites and output the
 * results on standard output.
 * 
 * Returns the total number of failed tests, i.e. zero if all tests were
 * passed.
 */
int run() {
  size_t totalPassed = 0u;
  size_t totalFailed = 0u;
  double totalDurationMicros = 0.0;
  
  for (auto& suite : yatest::TestSuites) {
    std::cout << "Running "
              << colorize("\033[1;36m", suite->name())
              << " [" << std::endl;
    auto result = suite->run();
    for (auto& testResult : result.testResults()) {
      switch (testResult.status)
      {
      case yatest::TestStatus::Passed:
        totalPassed += 1u;
        std::cout << "  " << colorize("\033[0;32m", "PASS") << " " << testResult.name
                  << " (" << std::fixed << std::setprecision(1) << testResult.durationMicros << " µs)"
                  << std::endl;
        break;
      case yatest::TestStatus::Failed:
        totalFailed += 1u;
        std::cout << "  " << colorize("\033[0;31m", "FAIL") << " " << testResult.name << " (" << testResult.what << ")"
                  << " (" << std::fixed << std::setprecision(1) << testResult.durationMicros << " µs)"
                  << std::endl;
        break;
      }
    }
    totalDurationMicros += result.durationMicros();
    std::cout << "] "
              << "(" << std::fixed << std::setprecision(1) << result.durationMicros() << " µs)"
              << std::endl;
  }

  std::cout << "\nTotal: " << totalPassed << " passed, " << totalFailed  << " failed"
            << " (" << std::fixed << std::setprecision(1) << totalDurationMicros << " µs)"
            << std::endl;

  return totalFailed;
}

}

#endif

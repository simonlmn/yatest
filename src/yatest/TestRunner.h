#ifndef YATEST_TESTRUNNER_H_
#define YATEST_TESTRUNNER_H_

#include "TestSuite.h"
#include <iostream>
#include <iomanip>

namespace yatest {

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
    std::cout << "Running " << suite->name() << " [" << std::endl;
    auto result = suite->run();
    for (auto& testResult : result.testResults()) {
      switch (testResult.status)
      {
      case yatest::TestStatus::Passed:
        totalPassed += 1u;
        std::cout << "  PASS " << testResult.name
                  << " (" << std::fixed << std::setprecision(1) << testResult.durationMicros << " µs)"
                  << std::endl;
        break;
      case yatest::TestStatus::Failed:
        totalFailed += 1u;
        std::cout << "  FAIL " << testResult.name << " (" << testResult.what << ")"
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

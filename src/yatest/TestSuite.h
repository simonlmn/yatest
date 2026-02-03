#ifndef YATEST_TESTSUITE_H_
#define YATEST_TESTSUITE_H_

#include <vector>
#include <functional>
#include <memory>
#include <string>
#include <chrono>

namespace yatest {

enum struct TestStatus {
  Passed,
  Failed
};

struct TestResult final {
  const char* name;
  TestStatus status;
  std::string what;
  double durationMicros;

  TestResult(const char* name, TestStatus status, std::string what, double durationMicros)
      : name(name), status(status), what(what), durationMicros(durationMicros) {}
};

class TestSuiteResult final {
  std::vector<TestResult> _testResults {};
  double _durationMicros = 0.0;

public:
  const std::vector<TestResult>& testResults() const { return _testResults; }
  double durationMicros() const { return _durationMicros; }
  void setDurationMicros(double durationMicros) { _durationMicros = durationMicros; }

  void passed(const char* name, double durationMicros) {
    _testResults.emplace_back(name, TestStatus::Passed, "", durationMicros);
  }

  void failed(const char* name, double durationMicros) {
    failed(name, "", durationMicros);
  }

  void failed(const char* name, const char* what, double durationMicros) {
    _testResults.emplace_back(name, TestStatus::Failed, what, durationMicros);
  }
};

struct ITestSuite {
  virtual ~ITestSuite() {}
  virtual const char* name() const = 0;
  virtual TestSuiteResult run() = 0;
};

class TestSuite final : public ITestSuite {
  const char* _name;
  std::vector<std::pair<const char*, std::function<void()>>> _tests {};

public:
  TestSuite(const char* name) : _name(name) {}

  TestSuite& tests(const char* name, std::function<void()> test) {
    _tests.emplace_back(std::make_pair(name, test));
    return *this;
  }

  const char* name() const override {
    return _name;
  }

  TestSuiteResult run() override {
    using Clock = std::chrono::steady_clock;
    using DurationMicros = std::chrono::duration<double, std::micro>;

    TestSuiteResult result;
    auto suiteStart = Clock::now();
    for (auto [name, test] : _tests) {
      auto testStart = Clock::now();
      try {
        test();
        auto testEnd = Clock::now();
        result.passed(name, DurationMicros(testEnd - testStart).count());
      } catch (std::exception& e) {
        auto testEnd = Clock::now();
        result.failed(name, e.what(), DurationMicros(testEnd - testStart).count());
      } catch (...) {
        auto testEnd = Clock::now();
        result.failed(name, DurationMicros(testEnd - testStart).count());
      }
    }
    auto suiteEnd = Clock::now();
    result.setDurationMicros(DurationMicros(suiteEnd - suiteStart).count());
    return result;
  }
};

inline std::vector<std::unique_ptr<ITestSuite>> TestSuites = {};

inline TestSuite& suite(const char* name) {
  return *static_cast<TestSuite*>(TestSuites.emplace_back(std::make_unique<TestSuite>(name)).get());
}

}

#endif

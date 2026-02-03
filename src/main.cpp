/*
 * Main test runner implementation for running all yatest test suites compiled/linked with it.
 * 
 * When building the test executable using the build-and-run.sh script, this is used as the main
 * entry point if no main() function is provided by the library/project.
 */

#include <yatest/TestRunner.h>
#include <cstring>
#include <cstdlib>

namespace {

bool parseBoolEnv(const char* value, bool defaultValue) {
  if (value == nullptr) {
    return defaultValue;
  }
  if (std::strcmp(value, "0") == 0 || std::strcmp(value, "false") == 0 || std::strcmp(value, "FALSE") == 0 ||
      std::strcmp(value, "no") == 0 || std::strcmp(value, "NO") == 0) {
    return false;
  }
  if (std::strcmp(value, "1") == 0 || std::strcmp(value, "true") == 0 || std::strcmp(value, "TRUE") == 0 ||
      std::strcmp(value, "yes") == 0 || std::strcmp(value, "YES") == 0) {
    return true;
  }
  return defaultValue;
}

}

int main(int argc, char** argv) {
  yatest::setUseColor(parseBoolEnv(std::getenv("YATEST_COLOR"), yatest::useColorOutput()));

  // Command-line override
  for (int i = 1; i < argc; ++i) {
    if (std::strcmp(argv[i], "--no-color") == 0 || std::strcmp(argv[i], "--nocolor") == 0) {
      yatest::setUseColor(false);
    } else if (std::strcmp(argv[i], "--color") == 0) {
      yatest::setUseColor(true);
    }
  }

  return yatest::run();
}
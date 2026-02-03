/*
 * Main test runner implementation for running all yatest test suites compiled/linked with it.
 * 
 * When building the test executable using the build-and-run.sh script, this is used as the main
 * entry point if no main() function is provided by the library/project.
 */

#include <yatest/TestRunner.h>

int main() {
  return yatest::run();
}
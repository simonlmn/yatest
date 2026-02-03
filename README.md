# yatest: Yet Another Test Framework

A lightweight test framework and comprehensive Arduino mocking library for host-based unit testing of Arduino libraries and projects.

**yatest** provides:
- **Test framework**: Simple assertion helpers (`yatest::expect`) and test runner (`yatest::TestRunner`)
- **Arduino mocks**: Full mock implementations of Arduino core APIs (`Arduino.h`, `String`, `Stream`, etc.)
- **Host-based testing**: Compile and run tests on your development machine (macOS, Linux, Windows*) without hardware
- **arduino-cli compatible**: Yatest can be installed and pulled in using arduino-cli and build/run tests closely in a way how arduino-cli works.

This library consolidates test framework and mocking capabilities into a single, reusable Arduino library that can be independently referenced by other libraries.

*) The provided build scripts are written in bash for Mac/Linux, but the library itself also works on Windows. You need to build the tests on your own though.

## Features

### Test Framework
- `yatest::expect`: Assertion helpers for test validation
- `yatest::TestRunner`: Simple test execution and reporting
- `yatest::TestSuite`: Organize related tests

### Arduino API Mocks
- **Arduino.h**: Core functions (`millis()`, `micros()`, `delay()`, `random()`, `map()`, etc.)
- **WString.h**: Full `String` class implementation
- **Stream.h**: Base stream class with parsing methods
- **Serial mocks**: `RingBuffer` and `SerialMock` for serial communication testing
- **PROGMEM support**: No-op macros for flash memory operations
- **Time control**: Manual time advancement for deterministic testing

## Limitations

- **No hardware I/O**: GPIO, SPI, I2C, etc. are not implemented
- **Simulated time**: Time does not advance automatically
- **Host-only**: Tests run on development machines, not on actual Arduino devices

For on-device testing, use specialized frameworks like AUnit.

## Installation

Note: if using the provided build/run script ([extras/yatest.sh](yatest.sh), see below), the latest version of yatest will be installed automatically in a local build directory. It still might make sense to install it manually and use that installation explicitly (with or without the script, see below for options on how to use these).

### Arduino Library Manager
Install via the Arduino Library Manager or arduino-cli:
```bash
arduino-cli lib install yatest
```

### Manual Installation
Clone or copy this repository to your Arduino libraries directory:
```bash
cd ~/Arduino/libraries  # or your libraries path
git clone https://github.com/simonlmn/yatest.git
```

## Usage

Note: yatest provides a basic build process and test runner which should allow to focus on writing only the actual test suites/cases with minimal boilerplate. However, if that setup does not work, it can be also only used in parts (e.g. the mocks) and leave out the rest.

### Project Structure Conventions

The libraries should follow the Arduino library conventions already. To use the build scripts and standard test runner, organize tests as follows:

```
mylib/
├── src/
│   ├── mylib.h
│   └── mylib/
│       ├── MyClass.h
│       └── MyClass.cpp
├── test/
│   └── test_MyClass.cpp  # Individual test suites
├── library.properties
└── yatest.sh             # Copy/paste from yatest/extras directory
```

### Compiling and Running Tests

Copy and paste the `yatest.sh` script from the `extras` directory to the root of your library and execute it (this script can be added and checked in with your library and should not require any further modification).

The script will take care of setting up a local build directory, make the arduino-cli use this for installing libraries, install the lastest yatest release into this and then run the `build-and-run.sh` from yatest for your library. This will also make sure that any dependencies mentioned in your `library.properties` are automatically installed and added to the compiler include paths and build arguments (for any .cpp files).

The build scripts will automatically detect if you already have defined a `main()` function in your test sources and will add the standard yetest runner otherwise.

`yatest.sh` will use mainly two environment variables (if present):
- `LIB_DIR`: the path to the root directory of the library under test. If not set, it will be assumed to the same directory as where `yatest.sh` is stored.
- `YATEST_DIR`: the path to the root directory of the yatest library itself. If not set, will be automatically installed locally (only if not already installed before).
  - `YATEST_FORCE_INSTALL=1` can be used to force a (re-) install of the locally installed version of yatest (e.g. to ensure the latest version is used).

### Basic Test Example (without using TestSuites and the TestRunner)

Create a tests.cpp in your library's `test/` directory:

```cpp
#include <yatest.h>
#include <Arduino.h>
#include <MyLibrary.h>

void test_my_feature() {
  // Your test code using standard assertions
  assert(someFunction() == 42);
}

int main() {
  test_my_feature();
  return 0;
}
```

### Writing Tests using TestSuite and TestRunner

Create a tests.cpp in your library's `test/` directory:

```cpp
#include <yatest.h>
#include <Arduino.h>
#include <MyLibrary.h>

namespace {
  // Define as many (or few) test suites as needed
  static const yatest::TestSuite& TestMyLibrary =
    yatest::suite("My Library")
        .tests("calling someFunction", []() {
          yatest::expect::that(someFunction() == 42, "some function should return 42");
        })
        /* chain more .tests(...) calls as needed to define more test cases */;
}
```

The anonymous namespace containing the test suite and test case definitions can also be split and put into separate source files. When building the tests (see below) these will automatically be picked up and run by the test runner.

## Provided Mocks

### Arduino

The library provides comprehensive Arduino API mocks that allow your code to compile and run natively:

```cpp
#include <yatest.h>
#include <Arduino.h>

void test_timing() {
  _test_millis = 0;
  unsigned long start = millis();
  assert(start == 0);
  
  // Simulate time passing
  _test_millis = 1000;
  assert(millis() == 1000);
  
  // Or use helper functions
  advanceTimeMs(500);
  assert(millis() == 1500);
}
```

### Serial Communication

For testing serial communication (e.g., with `serial-transport` library):

```cpp
#include <yatest.h>
#include <Arduino.h>

void test_serial() {
  RingBuffer rxBuffer, txBuffer;
  SerialMock serial(rxBuffer, txBuffer);
  
  serial.write((const uint8_t*)"test", 4);
  assert(serial.available() == 4);
}
```

## Support

If you want to support this project, you can:

 * use it and provide feedback
 * fix bugs
 * develop improvements or new features
 * [![Donate](https://img.shields.io/badge/Donate-with%20PayPal-blue.svg)](https://www.paypal.com/donate/?hosted_button_id=BSKYG5C8S8HVU)

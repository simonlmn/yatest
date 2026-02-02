# yatest: Yet Another Test Framework

A lightweight test framework and comprehensive Arduino mocking library for host-based unit testing of Arduino libraries and projects.

**yatest** provides:
- **Test framework**: Simple assertion helpers (`yatest::Expect`) and test runner (`yatest::TestRunner`)
- **Arduino mocks**: Full mock implementations of Arduino core APIs (`Arduino.h`, `String`, `Stream`, etc.)
- **Host-based testing**: Compile and run tests on your development machine (macOS, Linux, Windows) without hardware
- **arduino-cli compatible**: Tests can be compiled using arduino-cli with a host-capable platform

This library consolidates test framework and mocking capabilities into a single, reusable Arduino library that can be independently referenced by other libraries.

## Features

### Test Framework
- `yatest::Expect`: Assertion helpers for test validation
- `yatest::TestRunner`: Simple test execution and reporting
- `yatest::TestSuite`: Organize related tests

### Arduino API Mocks
- **Arduino.h**: Core functions (`millis()`, `micros()`, `delay()`, `random()`, `map()`, etc.)
- **WString.h**: Full `String` class implementation
- **Stream.h**: Base stream class with parsing methods
- **Serial mocks**: `RingBuffer` and `SerialMock` for serial communication testing
- **PROGMEM support**: No-op macros for flash memory operations
- **Time control**: Manual time advancement for deterministic testing

## Installation

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

### Basic Test Example

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

### Arduino Mocks

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

### Serial Communication Mocks

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

## Recommended Integration into Library Projects

## Project Structure

For libraries using yatest, organize tests as follows:

```
mylib/
├── src/
│   ├── mylib.h
│   └── mylib/
│       ├── MyClass.h
│       └── MyClass.cpp
├── test/
│   ├── tests.cpp         # Main test entry point
│   └── test_Feature.cpp  # Individual test files
├── library.properties
└── README.md
```

## Compiling and Running Tests

Copy and paste the `yatest.sh` script from the `extras` directory to the root of your library and execute it.

The script will take care of setting up a local build directory, make the arduino-cli use this for installing libraries, install the lastest yatest release into this and then run the `build-and-run.sh` from yatest for your library. This will also make sure that any dependencies mentioned in your `library.properties` are automatically installed and added to the compiler include paths and build arguments (for any .cpp files).

The `yatest.sh` can be added and checked in with your library and should not require any further modification.

## Limitations

- **No hardware I/O**: GPIO, SPI, I2C, etc. are not implemented
- **Simulated time**: Time does not advance automatically
- **Host-only**: Tests run on development machines, not on actual Arduino devices

For on-device testing, use specialized frameworks like AUnit.

## Support

If you want to support this project, you can:

 * use it and provide feedback
 * fix bugs
 * develop improvements or new features
 * [![Donate](https://img.shields.io/badge/Donate-with%20PayPal-blue.svg)](https://www.paypal.com/donate/?hosted_button_id=BSKYG5C8S8HVU)

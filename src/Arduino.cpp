#include "Arduino.h"

#include <algorithm>

unsigned long _test_millis = 0;
unsigned long _test_micros = 0;

namespace {
    int pinModes[GPIO_MOCK_MAX_PINS];
    int pinValues[GPIO_MOCK_MAX_PINS];
    std::size_t digitalWriteCalls = 0u;
    int lastDigitalWritePin = NOT_A_PIN;
    int lastDigitalWriteValue = 0;
    int lastPinModePin = NOT_A_PIN;
    int lastPinModeMode = 0;
}

void resetGpioMocks() {
  std::fill_n(pinModes, GPIO_MOCK_MAX_PINS, -1);
  std::fill_n(pinValues, GPIO_MOCK_MAX_PINS, 0);
  digitalWriteCalls = 0u;
  lastDigitalWritePin = NOT_A_PIN;
  lastDigitalWriteValue = 0;
  lastPinModePin = NOT_A_PIN;
  lastPinModeMode = 0;
}

void setDigitalReadValue(int pin, int value) {
  if (pin < 0 || static_cast<std::size_t>(pin) >= GPIO_MOCK_MAX_PINS) {
    return;
  }
  pinValues[pin] = value;
}

int getPinMode(int pin) {
  if (pin < 0 || static_cast<std::size_t>(pin) >= GPIO_MOCK_MAX_PINS) {
    return -1;
  }
  return pinModes[pin];
}

int getLastPinModePin() {
  return lastPinModePin;
}

int getLastPinModeMode() {
  return lastPinModeMode;
}

int getDigitalWriteValue(int pin) {
  if (pin < 0 || static_cast<std::size_t>(pin) >= GPIO_MOCK_MAX_PINS) {
    return 0;
  }
  return pinValues[pin];
}

int getLastDigitalWritePin() {
  return lastDigitalWritePin;
}

int getLastDigitalWriteValue() {
  return lastDigitalWriteValue;
}

std::size_t getDigitalWriteCallCount() {
  return digitalWriteCalls;
}

void pinMode(int pin, int mode) {
  if (pin < 0 || static_cast<std::size_t>(pin) >= GPIO_MOCK_MAX_PINS) {
    return;
  }
  pinModes[pin] = mode;
  lastPinModePin = pin;
  lastPinModeMode = mode;
}

int digitalRead(int pin) {
  if (pin < 0 || static_cast<std::size_t>(pin) >= GPIO_MOCK_MAX_PINS) {
    return 0;
  }
  return pinValues[pin];
}

void digitalWrite(int pin, int value) {
  if (pin < 0 || static_cast<std::size_t>(pin) >= GPIO_MOCK_MAX_PINS) {
    return;
  }
  pinValues[pin] = value;
  lastDigitalWritePin = pin;
  lastDigitalWriteValue = value;
  digitalWriteCalls += 1u;
}

#!/bin/bash
# Build and run tests for a library.

set -e

LIB_DIR="$1"
if [ -z "$LIB_DIR" -o ! -d "$LIB_DIR" ]; then
    echo "Error: Library directory not specified."
    exit 1
fi

SRC_DIR="$LIB_DIR/src"
TEST_DIR="$LIB_DIR/test"
BUILD_DIR="$LIB_DIR/build"

YATEST_SRC_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "Building tests for library in $LIB_DIR..."
echo "Sources: $SRC_DIR"
echo "Tests: $TEST_DIR"

# Read depends from library.properties (format: depends=lib1, lib2 (>=0.1.2), ...)
DEPS_INCLUDES=""
DEPS_SOURCES=""
LIB_PROPERTIES="$LIB_DIR/library.properties"
if [ -f "$LIB_PROPERTIES" ]; then
    DEPENDENCIES_LINE=$(grep "^depends=" "$LIB_PROPERTIES" || true)
    DEPENDENCIES=${DEPENDENCIES_LINE#depends=}
    IFS=',' read -r -a DEP_ARRAY <<< "$DEPENDENCIES"

    # Install dependencies
    for DEP in "${DEP_ARRAY[@]}"; do
        DEP_NAME=$(echo "$DEP" | cut -d' ' -f1 | xargs)  # Get name before any version specifier
        if [ -n "$DEP_NAME" ]; then
            echo "Installing dependency: $DEP_NAME"
            arduino-cli lib install "$DEP_NAME"
            DEP_DIR="$(cd "$ARDUINO_DIRECTORIES_USER/libraries/$DEP_NAME" && pwd)"
            if [ ! -d "$DEP_DIR" ]; then
                echo "Error: dependency library not found at $DEP_DIR"
                exit 1
            fi
            
            DEPS_INCLUDES="$DEPS_INCLUDES -I$DEP_DIR/src"
            DEPS_SOURCES="$DEPS_SOURCES $(find "$DEP_DIR/src" -name "*.cpp" 2>/dev/null || true)"
        fi
    done
fi

# Compiler settings
CXX="${CXX:-clang++}"
CXXFLAGS="-std=c++17 -g -Wall -Wextra"

# Include paths
INCLUDES="-I$YATEST_SRC_DIR -I$SRC_DIR -I$TEST_DIR $DEPS_INCLUDES"

# Source files
YATEST_SOURCES=$(find "$YATEST_SRC_DIR" -name "*.cpp" 2>/dev/null || true)
LIB_SOURCES=$(find "$SRC_DIR" -name "*.cpp" 2>/dev/null || true)
TEST_SOURCES=$(find "$TEST_DIR" -name "*.cpp" 2>/dev/null)

# Compile and run tests
mkdir -p "$BUILD_DIR"

output="$BUILD_DIR/tests"

echo "Building tests..."
echo $CXX $CXXFLAGS $INCLUDES $YATEST_SOURCES $DEPS_SOURCES $LIB_SOURCES $TEST_SOURCES -o $output
if $CXX $CXXFLAGS $INCLUDES $YATEST_SOURCES $DEPS_SOURCES $LIB_SOURCES $TEST_SOURCES -o "$output"; then
    echo "Running tests..."
    if "$output"; then
        echo "✓ tests passed"
    else
        echo "✗ tests failed"
    fi
else
    echo "✗ tests compilation failed"
fi

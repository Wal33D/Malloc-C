#!/bin/sh
set -e

# Build the memory_test program
make memory_test > /dev/null

# Run the program and capture its output
output=$(./memory_test)

if [ "$output" = "All tests passed" ]; then
    echo "Memory tests passed: $output"
else
    echo "Memory tests failed: $output" >&2
    exit 1
fi

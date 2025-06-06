#!/bin/sh
set -e

# Build the example program
make example > /dev/null

# Run the program and capture its output
output=$(./example)

if [ "$output" = "Value: 42" ]; then
    echo "Test passed: $output"
else
    echo "Test failed: expected 'Value: 42', got '$output'" >&2
    exit 1
fi

#!/usr/bin/env sh
set -eu

# Determine the path to the built executable
exe="${1:-./build/example}"

# Run the program and capture its output
output=$("$exe")

if [ "$output" = "Value: 42" ]; then
    echo "Test passed: $output"
else
    echo "Test failed: expected 'Value: 42', got '$output'" >&2
    exit 1
fi

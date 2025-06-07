#!/usr/bin/env sh
set -eu

# Determine the path to the built test binary
exe="${1:-./build/memory_test}"

# Run the program and capture its output
output=$("$exe")

if [ "$output" = "All tests passed" ]; then
    echo "Memory tests passed: $output"
else
    echo "Memory tests failed: $output" >&2
    exit 1
fi

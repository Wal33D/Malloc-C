#!/usr/bin/env sh
set -eu
# Enable pipefail when available for better error handling
if (set -o pipefail) 2>/dev/null; then
    set -o pipefail
fi

# Build the memory_test program from a clean state
make clean > /dev/null
make memory_test > /dev/null

# Run the program and capture its output
output=$(./memory_test)

if [ "$output" = "All tests passed" ]; then
    echo "Memory tests passed: $output"
else
    echo "Memory tests failed: $output" >&2
    exit 1
fi

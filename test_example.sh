#!/usr/bin/env sh
set -eu
# Enable pipefail when available for better error handling
if (set -o pipefail) 2>/dev/null; then
    set -o pipefail
fi

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

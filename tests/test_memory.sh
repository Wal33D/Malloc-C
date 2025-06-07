#!/usr/bin/env sh
set -eu
# Enable pipefail when available for better error handling
if (set -o pipefail) 2>/dev/null; then
    set -o pipefail
fi

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


#!/bin/bash

# This script is the entrypoint for the Docker container. 
# It is responsible for building the project and running the tests with Valgrind.
set -e  # Exit on error

LOG_DIR="/app/tmp"
LOG_FILE="valgrind.log"

mkdir -p "$LOG_DIR"

if [[ $# -eq 0 ]]; then
    echo "📦 Building the project..."
    make -C /app/test clean
    make -C /app/test

    echo "🚀 Running tests with Valgrind..."
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file="$LOG_DIR/base-$LOG_FILE" --num-callers=50 /app/bin/arisr_test
    valgrind --tool=massif --stacks=yes --massif-out-file=/app/tmp/massif.out --log-file="$LOG_DIR/massif-$LOG_FILE" /app/bin/arisr_test

    echo "✅ Execution completed. Log available at: $LOG_FILE"
else
    echo "🔹 Custom command detected: $@"
    bash -c "$@"
fi


echo "🔹 Starting bash..."
# Call bash
bash

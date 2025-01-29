#!/bin/bash

PROGRAM="./build/task"
TEST_DIR="./tests"

for test_file in "$TEST_DIR"/test[0-9].txt; do
    test_number=$(basename "$test_file" .txt | sed 's/test//g')

    result_file="$TEST_DIR/test${test_number}_result.txt"
    if [[ ! -f "$result_file" ]]; then
        echo "Expected result file for test $test_number not found: $result_file"
        continue
    fi

    actual_output=$($PROGRAM "$test_file")
    temp_file=$(mktemp)
    echo "$actual_output" > "$temp_file"

    if diff -q "$temp_file" "$expected_result_file" > /dev/null; then
        echo "Test $test_number: PASSED"
    else
        echo "Test $test_number: FAILED"
        echo "Expected:"
        cat "$expected_result_file"
        echo "Actual:"
        cat "$temp_file"
    fi

    rm "$temp_file"
done

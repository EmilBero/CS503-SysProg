#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suite in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "cd with argument changes directory" {
    # Create a temporary directory for the test.
    tmp=$(mktemp -d)
    run ./dsh <<EOF
cd $tmp
pwd
exit
EOF
    [ "$status" -eq 0 ]
    # Check that the output of pwd contains the temporary directory path.
    echo "$output" | grep -q "$tmp"
    rm -rf "$tmp"
}

@test "cd with no arguments does nothing" {
    current=$(pwd)
    run ./dsh <<EOF
cd
pwd
exit
EOF
    [ "$status" -eq 0 ]
    # The output of pwd should contain the current working directory.
    echo "$output" | grep -q "$current"
}

@test "external command echo prints arguments" {
    run ./dsh <<EOF
echo Hello, world!
exit
EOF
    [ "$status" -eq 0 ]
    # Check that the output contains the text "Hello, world!"
    echo "$output" | grep -q "Hello, world!"
}

@test "handles quoted strings correctly" {
    run ./dsh <<EOF
echo "   hello   world  "
exit
EOF
    [ "$status" -eq 0 ]
    # The output should preserve the inner spaces.
    echo "$output" | grep -q "   hello   world  "
}

@test "nonexistent command returns error message" {
    run ./dsh <<EOF
nonexistent_command
exit
EOF
    # We expect an error message from execvp (using perror), which should contain "execvp"
    echo "$output" | grep -q "execvp"
}

@test "extra whitespace is handled correctly" {
    run ./dsh <<EOF
    echo       hello       world    
exit
EOF
    [ "$status" -eq 0 ]
    # The output should contain "hello world" (normalized).
    # Depending on your implementation, you might allow multiple spaces inside the output.
    # Here we check that "hello" and "world" appear in order.
    echo "$output" | grep -q "hello"
    echo "$output" | grep -q "world"
}

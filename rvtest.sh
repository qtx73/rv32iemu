#!/bin/bash

test_dir="riscv-tests/"

# "fence" を含むテストを除外
test_files=$(find "$test_dir" -type f -name "rv32ui-p-*" ! -name "*.dump" ! -name "*fence*")

for test_file in $test_files; do
    #riscv64-unknown-elf-gcc -march=rv32i -mabi=ilp32 -nostdlib -nostartfiles -o program.elf start.s $test_file
    #riscv64-unknown-elf-objcopy -O binary program.elf program.bin
    #hexdump -v -e '"%08x\n"' program.bin > program.hex
    cp "$test_file" program.hex
    ./a.out program.hex
    result=$(echo $?)
    expected=1

    if [ "$result" = "$expected" ]; then
        echo "Test passed for $test_file"
    else
        echo "Test failed for $test_file"
        exit 1
    fi
done

echo "All tests passed successfully."

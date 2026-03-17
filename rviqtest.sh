#!/bin/bash

riscv64-unknown-elf-gcc -march=rv32i_zicsr -mabi=ilp32 -nostdlib -nostartfiles -o program.elf start.s tests/test_iq.c 
riscv64-unknown-elf-objcopy -O binary program.elf program.bin
hexdump -v -e '"%08x\n"' program.bin > program.hex
./core program.hex
result=$(echo $?)

expected=42

if [ "$result" = "$expected" ]; then
    echo "Test passed"
else
    echo "Test failed"
    exit 1
fi

echo "All tests passed successfully."


#!/bin/bash
set -e

# Compile the test program
# Added _zicsr to explicitly enable CSR instructions
echo "1. Compiling test_csr.c..."
riscv64-unknown-elf-gcc -O2 -march=rv32i_zicsr -mabi=ilp32 -nostdlib -Ttext=0x0 tests/test_csr.c -o test_csr.elf

# Extract raw binary
echo "2. Extracting raw binary..."
riscv64-unknown-elf-objcopy -O binary test_csr.elf test_csr.bin

# Convert binary to hex strings (32-bit words per line, little-endian format)
echo "3. Converting to hex file..."
hexdump -v -e '1/4 "%08x\n"' test_csr.bin > test_csr.hex

# Compile the emulator 
# REPLACE 'emulator.c' WITH YOUR ACTUAL C FILE NAME (e.g., main.c)
# echo "4. Compiling emulator..."
# gcc core.c -o emu
# gcc -Wall -Wextra -o core core.c

# Run the emulator with the hex file and catch exit codes
echo "5. Running test..."
set +e
./core test_csr.hex
EXIT_CODE=$?
set -e

# Verify the result
echo "---------------------------------"
if [ $EXIT_CODE -eq 0 ]; then
    echo "Result: SUCCESS! (Exit code: 0)"
    echo "All CSR instructions worked as expected."
else
    echo "Result: FAILED! (Exit code: $EXIT_CODE)"
    echo "Check test_csr.c to see which test corresponds to this error code."
fi
// Provide a naked entry point to avoid C runtime (crt0) initialization overhead
// and set the stack pointer manually.
__attribute__((naked, section(".text.init"))) void _start() {
    asm volatile (
        "li sp, 0x40000 \n\t" // Initialize SP to the end of 256KB memory
        "j main \n\t"
    );
}

// Helper function to exit the emulator.
// The emulator uses xreg[3] (gp) as the exit code during ECALL.
void exit_emu(int code) {
    asm volatile (
        "mv x3, %0 \n\t"
        "ecall \n\t"
        : : "r" (code)
    );
}

int main() {
    unsigned int read_val;

    // Test 1: CSRRW (Write and Read)
    // Write 0xAAAA to mscratch (CSR 0x340)
    asm volatile (
        "li t0, 0xAAAA \n\t"
        "csrrw zero, 0x340, t0 \n\t"
        "csrrs %0, 0x340, zero \n\t" // Read without modifying
        : "=r" (read_val) : : "t0"
    );
    // If read_val is not 0xAAAA, exit with error code 1
    if (read_val != 0xAAAA) exit_emu(1);

    // Test 2: CSRRS (Read and Set bits)
    // Set bit 0 (0x0001) -> Expected: 0xAAAB
    asm volatile (
        "li t0, 0x0001 \n\t"
        "csrrs zero, 0x340, t0 \n\t"
        "csrrs %0, 0x340, zero \n\t" // Read without modifying
        : "=r" (read_val) : : "t0"
    );
    // If read_val is not 0xAAAB, exit with error code 2
    if (read_val != 0xAAAB) exit_emu(2);

    // Test 3: CSRRC (Read and Clear bits)
    // Clear bit 1 and 3 (0x000A) -> 0xAAAB & ~0xA -> Expected: 0xAAA1
    asm volatile (
        "li t0, 0x000A \n\t"
        "csrrc zero, 0x340, t0 \n\t"
        "csrrs %0, 0x340, zero \n\t" // Read without modifying
        : "=r" (read_val) : : "t0"
    );
    // If read_val is not 0xAAA1, exit with error code 3
    if (read_val != 0xAAA1) exit_emu(3);

    // Test 4: CSRRWI (Read and Write Immediate)
    // Write immediate 0x1F (31) to mscratch
    asm volatile (
        "csrrwi zero, 0x340, 0x1F \n\t"
        "csrrs %0, 0x340, zero \n\t" // Read without modifying
        : "=r" (read_val)
    );
    // If read_val is not 0x001F, exit with error code 4
    if (read_val != 0x001F) exit_emu(4);

    // All tests passed successfully, exit with code 0
    exit_emu(0);

    return 0;
}
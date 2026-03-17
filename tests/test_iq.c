#include <stdint.h>

#define CSR_MSTATUS 0x300
#define CSR_MTVEC   0x305

#define write_csr(csr, value) \
    asm volatile ("csrw %0, %1" : : "i"(csr), "r"(value))

uint32_t flag = 0;

__attribute__((interrupt("machine")))
void irq_handler() {
    flag = 1;
}

int main() {
    // Set the interrupt vector to the handler
    write_csr(CSR_MTVEC, (uint32_t)irq_handler);

    write_csr(CSR_MSTATUS, 0x8); // Enable machine interrupts

    while (flag == 1) {
        // Wait for interrupt
        asm volatile ("nop");
    }

    return 42;
}

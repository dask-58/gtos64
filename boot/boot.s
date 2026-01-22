.section .text.boot
.global _start

_start:
    /* Check current exception level */
    mrs x0, CurrentEL
    and x0, x0, #12
    cmp x0, #4
    b.eq el1_entry
    
    cmp x0, #8
    b.eq el2_to_el1
    
    cmp x0, #12
    b.eq el3_to_el2
    
    /* If EL0, hang */
    b hang

el3_to_el2:
    /* Configure SCR_EL3 */
    mrs x0, SCR_EL3
    orr x0, x0, #(1 << 10)
    orr x0, x0, #(1 << 0)
    msr SCR_EL3, x0
    
    /* Set SPSR_EL3 */
    mov x0, #0x3c9
    msr SPSR_EL3, x0
    
    /* Return to EL2 address */
    adr x0, el2_to_el1
    msr ELR_EL3, x0
    eret

el2_to_el1:
    /* Configure HCR_EL2 */
    mov x0, #(1 << 31)
    msr HCR_EL2, x0
    
    /* Set SPSR_EL2 */
    mov x0, #0x3c5
    msr SPSR_EL2, x0
    
    /* Return to EL1 address */
    adr x0, el1_entry
    msr ELR_EL2, x0
    eret

el1_entry:
    /* Set stack pointer */
    ldr x0, =0x40400000
    mov sp, x0
    
    /* Clear BSS */
    ldr x0, =__bss_start
    ldr x1, =__bss_end
    mov x2, #0

bss_loop:
    cmp x0, x1
    b.ge call_kernel
    str x2, [x0], #8
    b bss_loop

call_kernel:
    /* Branch to C kernel */
    bl kernel_main

hang:
    /* Infinite loop if kernel returns */
    b hang
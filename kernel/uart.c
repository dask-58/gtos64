#include "../include/uart.h"

// PL011 UART registers
#define UART0_BASE 0x09000000

#define UART0_DR   ((volatile unsigned int*)(UART0_BASE + 0x00))
#define UART0_FR   ((volatile unsigned int*)(UART0_BASE + 0x18))
#define UART0_IBRD ((volatile unsigned int*)(UART0_BASE + 0x24))
#define UART0_FBRD ((volatile unsigned int*)(UART0_BASE + 0x28))
#define UART0_LCR_H ((volatile unsigned int*)(UART0_BASE + 0x2C))
#define UART0_CR   ((volatile unsigned int*)(UART0_BASE + 0x30))

#define FR_TXFF (1 << 5)  // Transmit FIFO full
#define FR_RXFE (1 << 4)  // Receive FIFO empty
#define FR_BUSY (1 << 3)  // UART busy

#define CR_UART_EN (1 << 0)  // UART enable
#define CR_TX_EN   (1 << 8)  // Transmit enable
#define CR_RX_EN   (1 << 9)  // Receive enable

#define LCR_FEN    (1 << 4)  // Enable FIFOs
#define LCR_WLEN_8 (3 << 5)  // 8 bits

void uart_init(void) {
    *UART0_CR = 0;
    
    while (*UART0_FR & FR_BUSY);
    
    *UART0_LCR_H &= ~LCR_FEN;
    
    *UART0_IBRD = 13;
    *UART0_FBRD = 1;  
    
    *UART0_LCR_H = LCR_WLEN_8 | LCR_FEN;
    
    *UART0_CR = CR_UART_EN | CR_TX_EN | CR_RX_EN;
}

void uart_putc(char c) {
    while (*UART0_FR & FR_TXFF);
    
    if (c == '\n') {
        *UART0_DR = '\r';
        while (*UART0_FR & FR_TXFF);
    }
    
    *UART0_DR = c;
}

void uart_puts(const char* str) {
    while (*str) {
        uart_putc(*str++);
    }
}

char uart_getc(void) {
    while (*UART0_FR & FR_RXFE);
    
    return (char)(*UART0_DR & 0xFF);
}
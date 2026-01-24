#include "../include/input.h"
#include "../include/uart.h"

void readline(char* buffer, int max_length) {
    int index = 0;
    
    while (1) {
        char c = uart_getc();
        
        if (c == '\n' || c == '\r') {
            buffer[index] = '\0';
            uart_putc('\n');
            return;
        }
        
        if (c == 0x7F || c == 0x08) {
            if (index > 0) {
                index--;
                uart_putc('\b');
                uart_putc(' ');
                uart_putc('\b');
            }
            continue;
        }
        
        if (c >= 32 && c < 127) {
            if (index < max_length - 1) {
                buffer[index++] = c;
                uart_putc(c);
            }
        }
    }
}

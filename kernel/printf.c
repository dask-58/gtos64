#include "../include/printf.h"
#include "../include/uart.h"

static void print_uint_base(unsigned int num, int base, int uppercase) {
    char buffer[32];
    int i = 0;
    
    if (num == 0) {
        uart_putc('0');
        return;
    }
    
    const char* digits = uppercase ? "0123456789ABCDEF" : "0123456789abcdef";
    
    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
    }
    
    while (i > 0) {
        uart_putc(buffer[--i]);
    }
}

void print_str(const char* str) {
    if (str) {
        uart_puts(str);
    } else {
        uart_puts("(null)");
    }
}

void print_char(char c) {
    uart_putc(c);
}

void print_int(int num) {
    if (num < 0) {
        uart_putc('-');
        num = -num;
    }
    print_uint_base((unsigned int)num, 10, 0);
}

void print_uint(unsigned int num) {
    print_uint_base(num, 10, 0);
}

void print_hex(unsigned int num) {
    uart_puts("0x");
    print_uint_base(num, 16, 0);
}

void print_ptr(void* ptr) {
    unsigned long addr = (unsigned long)ptr;
    uart_puts("0x");
    
    if (addr == 0) {
        uart_putc('0');
        return;
    }
    
    char buffer[16];
    int i = 0;
    const char* digits = "0123456789abcdef";
    
    while (addr > 0) {
        buffer[i++] = digits[addr & 0xF];
        addr >>= 4;
    }
    
    while (i > 0) {
        uart_putc(buffer[--i]);
    }
}
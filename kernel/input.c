#include "../include/input.h"
#include "../include/uart.h"
#include "../include/string.h"
#include "../include/printf.h"

void readline(char* buffer, int max_length, char (*history)[128], int history_count, completion_callback_t completer) {
    int index = 0;
    int history_idx = history_count;
    char current_line_buf[128];
    current_line_buf[0] = '\0';
    
    buffer[0] = '\0';
    
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
                buffer[index] = '\0';
            }
            continue;
        }
        
        if (c == '\t') {
            if (completer) {
                buffer[index] = '\0';
                completer(buffer, &index, max_length);
            }
            continue;
        }
        
        if (c == '\033') {
            char next1 = uart_getc();
            if (next1 == '[') {
                char next2 = uart_getc();
                if (next2 == 'A') {
                    if (history_count > 0) {
                        if (history_idx == history_count) {
                            buffer[index] = '\0';
                            strcpy(current_line_buf, buffer);
                        }
                        
                        if (history_idx > 0) {
                            history_idx--;
                            
                            while (index > 0) {
                                uart_putc('\b');
                                uart_putc(' ');
                                uart_putc('\b');
                                index--;
                            }
                            
                            strcpy(buffer, history[history_idx]);
                            index = strlen(buffer);
                            uart_puts(buffer);
                        }
                    }
                } else if (next2 == 'B') {
                    if (history_count > 0 && history_idx < history_count) {
                        history_idx++;
                        
                        while (index > 0) {
                            uart_putc('\b');
                            uart_putc(' ');
                            uart_putc('\b');
                            index--;
                        }
                        
                        if (history_idx == history_count) {
                            strcpy(buffer, current_line_buf);
                        } else {
                            strcpy(buffer, history[history_idx]);
                        }
                        
                        index = strlen(buffer);
                        uart_puts(buffer);
                    }
                }
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

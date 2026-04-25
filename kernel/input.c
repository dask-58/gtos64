#include "../include/input.h"
#include "../include/uart.h"
#include "../include/string.h"
#include "../include/printf.h"

typedef enum {
    VT100_NONE = 0,
    VT100_UP,
    VT100_DOWN,
    VT100_LEFT,
    VT100_RIGHT,
    VT100_HOME,
    VT100_END,
    VT100_DELETE
} vt100_key_t;

static void redraw_line(const char* prompt, const char* buffer, int length,
                        int cursor) {
    uart_putc('\r');
    uart_puts(prompt);

    for (int i = 0; i < length; i++) {
        uart_putc(buffer[i]);
    }

    print_str("\033[K");

    while (length > cursor) {
        uart_putc('\b');
        length--;
    }
}

static void replace_line(char* buffer, int max_length, const char* prompt,
                         const char* value, int* length, int* cursor) {
    strlcpy(buffer, value ? value : "", (size_t)max_length);
    *length = (int)strlen(buffer);
    *cursor = *length;
    redraw_line(prompt, buffer, *length, *cursor);
}

static vt100_key_t parse_csi_sequence(void) {
    int param = 0;
    char c;

    while (1) {
        c = uart_getc();

        if (c >= '0' && c <= '9') {
            param = (param * 10) + (c - '0');
            continue;
        }

        if (c == ';') {
            continue;
        }

        break;
    }

    switch (c) {
        case 'A':
            return VT100_UP;
        case 'B':
            return VT100_DOWN;
        case 'C':
            return VT100_RIGHT;
        case 'D':
            return VT100_LEFT;
        case 'H':
            return VT100_HOME;
        case 'F':
            return VT100_END;
        case '~':
            if (param == 1 || param == 7) {
                return VT100_HOME;
            }
            if (param == 3) {
                return VT100_DELETE;
            }
            if (param == 4 || param == 8) {
                return VT100_END;
            }
            return VT100_NONE;
        default:
            return VT100_NONE;
    }
}

static vt100_key_t read_vt100_key(void) {
    char introducer = uart_getc();

    if (introducer == '[') {
        return parse_csi_sequence();
    }

    if (introducer == 'O') {
        char c = uart_getc();

        if (c == 'H') {
            return VT100_HOME;
        }
        if (c == 'F') {
            return VT100_END;
        }
    }

    return VT100_NONE;
}

void readline(char* buffer, int max_length, const char* prompt,
              int history_count, history_getter_t history_getter,
              completion_callback_t completer) {
    int length = 0;
    int cursor = 0;
    int history_idx = history_count;
    char current_line_buf[max_length];
    current_line_buf[0] = '\0';
    
    buffer[0] = '\0';
    
    while (1) {
        char c = uart_getc();
        
        if (c == '\n' || c == '\r') {
            buffer[length] = '\0';
            uart_putc('\n');
            return;
        }
        
        if (c == 0x7F || c == 0x08) {
            if (cursor > 0) {
                if (cursor == length) {
                    cursor--;
                    length--;
                    buffer[length] = '\0';
                    uart_putc('\b');
                    uart_putc(' ');
                    uart_putc('\b');
                } else {
                    memmove(&buffer[cursor - 1], &buffer[cursor],
                            (size_t)(length - cursor + 1));
                    cursor--;
                    length--;
                    redraw_line(prompt, buffer, length, cursor);
                }
            }
            continue;
        }
        
        if (c == '\t') {
            if (completer) {
                completer(buffer, &length, &cursor, max_length);
                if (length >= max_length) {
                    length = max_length - 1;
                    buffer[length] = '\0';
                }
                if (cursor > length) {
                    cursor = length;
                }
                redraw_line(prompt, buffer, length, cursor);
            }
            continue;
        }
        
        if (c == '\033') {
            switch (read_vt100_key()) {
                case VT100_UP:
                    if (history_count > 0 && history_getter) {
                        if (history_idx == history_count) {
                            strlcpy(current_line_buf, buffer,
                                    (size_t)max_length);
                        }

                        if (history_idx > 0) {
                            history_idx--;
                            replace_line(buffer, max_length, prompt,
                                         history_getter(history_idx), &length,
                                         &cursor);
                        }
                    }
                    break;
                case VT100_DOWN:
                    if (history_count > 0 && history_idx < history_count) {
                        history_idx++;
                        if (history_idx == history_count) {
                            replace_line(buffer, max_length, prompt,
                                         current_line_buf, &length, &cursor);
                        } else {
                            replace_line(buffer, max_length, prompt,
                                         history_getter(history_idx), &length,
                                         &cursor);
                        }
                    }
                    break;
                case VT100_LEFT:
                    if (cursor > 0) {
                        uart_putc('\b');
                        cursor--;
                    }
                    break;
                case VT100_RIGHT:
                    if (cursor < length) {
                        uart_putc(buffer[cursor]);
                        cursor++;
                    }
                    break;
                case VT100_HOME:
                    while (cursor > 0) {
                        uart_putc('\b');
                        cursor--;
                    }
                    break;
                case VT100_END:
                    while (cursor < length) {
                        uart_putc(buffer[cursor]);
                        cursor++;
                    }
                    break;
                case VT100_DELETE:
                    if (cursor < length) {
                        memmove(&buffer[cursor], &buffer[cursor + 1],
                                (size_t)(length - cursor));
                        length--;
                        redraw_line(prompt, buffer, length, cursor);
                    }
                    break;
                case VT100_NONE:
                    break;
            }
            continue;
        }
        
        if (c >= 32 && c < 127) {
            if (length < max_length - 1) {
                if (cursor == length) {
                    buffer[cursor++] = c;
                    length++;
                    buffer[length] = '\0';
                    uart_putc(c);
                } else {
                    memmove(&buffer[cursor + 1], &buffer[cursor],
                            (size_t)(length - cursor + 1));
                    buffer[cursor] = c;
                    cursor++;
                    length++;
                    redraw_line(prompt, buffer, length, cursor);
                }
                history_idx = history_count;
            }
        }
    }
}

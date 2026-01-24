#ifndef PRINTF_H
#define PRINTF_H

void print_str(const char *str);
void print_char(char c);
void print_int(int num);
void print_uint(unsigned int num);
void print_hex(unsigned int num);
void print_ptr(void *ptr);

#define printf_simple(str) print_str(str)
#define printf_1d(str, num)                                                    \
  do {                                                                         \
    print(str);                                                            \
    print(num);                                                            \
  } while (0)
#define printf_1x(str, num)                                                    \
  do {                                                                         \
    print_str(str);                                                            \
    print_hex(num);                                                            \
  } while (0)
#define printf_1s(str1, str2)                                                  \
  do {                                                                         \
    print_str(str1);                                                           \
    print_str(str2);                                                           \
  } while (0)
#define printf_1p(str, ptr)                                                    \
  do {                                                                         \
    print_str(str);                                                            \
    print_ptr(ptr);                                                            \
  } while (0)

// ANSI color codes
#define COLOR_RESET "\033[0m"
#define COLOR_BLACK "\033[30m"
#define COLOR_RED "\033[31m"
#define COLOR_GREEN "\033[32m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_BLUE "\033[34m"
#define COLOR_MAGENTA "\033[35m"
#define COLOR_CYAN "\033[36m"
#define COLOR_WHITE "\033[37m"

#define COLOR_BRIGHT_BLACK "\033[90m"
#define COLOR_BRIGHT_RED "\033[91m"
#define COLOR_BRIGHT_GREEN "\033[92m"
#define COLOR_BRIGHT_YELLOW "\033[93m"
#define COLOR_BRIGHT_BLUE "\033[94m"
#define COLOR_BRIGHT_MAGENTA "\033[95m"
#define COLOR_BRIGHT_CYAN "\033[96m"
#define COLOR_BRIGHT_WHITE "\033[97m"

#define STYLE_BOLD "\033[1m"
#define STYLE_DIM "\033[2m"
#define STYLE_UNDERLINE "\033[4m"

#endif
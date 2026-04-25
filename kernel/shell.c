#include "../include/shell.h"
#include "../include/input.h"
#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/string.h"

#define MAX_HISTORY 10
#define INPUT_BUFFER_SIZE 128
#define STACK_TOP ((void*)0x41000000)

static const char shell_prompt[] = COLOR_BRIGHT_CYAN
                                   "gtos64"
                                   COLOR_WHITE
                                   "@"
                                   COLOR_BRIGHT_YELLOW
                                   "kernel"
                                   COLOR_BRIGHT_WHITE
                                   " $ "
                                   COLOR_RESET;

static char history[MAX_HISTORY][INPUT_BUFFER_SIZE];
static int history_head = 0;
static int history_count = 0;

typedef void (*cmd_handler_t)(const char* args);

typedef struct {
    const char* name;
    const char* description;
    cmd_handler_t handler;
} command_t;

static void cmd_help(const char* args);
static void cmd_clear(const char* args);
static void cmd_echo(const char* args);
static void cmd_about(const char* args);
static void cmd_uptime(const char* args);
static void cmd_sysinfo(const char* args);
static void cmd_history(const char* args);
static void cmd_meminfo(const char* args);
static void cmd_ascii(const char* args);

static const command_t commands[] = {
    {"help", "Show available commands", cmd_help},
    {"clear", "Clear the screen", cmd_clear},
    {"echo", "Print text to screen", cmd_echo},
    {"about", "About this operating system", cmd_about},
    {"uptime", "Show system uptime", cmd_uptime},
    {"sysinfo", "Display system information", cmd_sysinfo},
    {"history", "Show command history", cmd_history},
    {"meminfo", "Show memory layout", cmd_meminfo},
    {"ascii", "Show ASCII art", cmd_ascii},
};

static const int num_commands = sizeof(commands) / sizeof(command_t);

static int history_slot(int history_index) {
    return (history_head + history_index) % MAX_HISTORY;
}

static const char* history_get(int history_index) {
    if (history_index < 0 || history_index >= history_count) {
        return 0;
    }

    return history[history_slot(history_index)];
}

static void history_push(const char* input) {
    int slot;

    if (!input || !*input) {
        return;
    }

    if (history_count > 0 &&
        strcmp(history_get(history_count - 1), input) == 0) {
        return;
    }

    if (history_count < MAX_HISTORY) {
        slot = history_slot(history_count);
        history_count++;
    } else {
        slot = history_head;
        history_head = (history_head + 1) % MAX_HISTORY;
    }

    strlcpy(history[slot], input, sizeof(history[slot]));
}

static void shell_completer(char* buffer, int* length, int* cursor,
                            int max_len) {
    int matches = 0;
    int match_idx = -1;
    size_t prefix_len;

    if (*cursor != *length || strchr(buffer, ' ') != 0) {
        return;
    }

    prefix_len = strlen(buffer);
    
    for (int i = 0; i < num_commands; i++) {
        if (strncmp(buffer, commands[i].name, prefix_len) == 0) {
            matches++;
            match_idx = i;
        }
    }
    
    if (matches == 1) {
        const char* name = commands[match_idx].name;
        size_t name_len = strlen(name);

        if (name_len < (size_t)max_len) {
            strlcpy(buffer, name, (size_t)max_len);
            *length = (int)strlen(buffer);
            *cursor = *length;

            if (*length < max_len - 1) {
                buffer[*length] = ' ';
                (*length)++;
                buffer[*length] = '\0';
                *cursor = *length;
            }
        }
    }
}

void cmd_help(const char* args) {
    (void)args;
    
    print_str(COLOR_BRIGHT_WHITE);
    print_str(STYLE_BOLD);
    print_str("\nAvailable Commands:\n");
    print_str(COLOR_RESET);
    print_str(COLOR_CYAN);
    print_str("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    print_str(COLOR_RESET);
    
    for (int i = 0; i < num_commands; i++) {
        print_str(COLOR_BRIGHT_GREEN);
        print_str("  ");
        print_str(commands[i].name);
        print_str(COLOR_RESET);
        int len = strlen(commands[i].name);
        for(int j=0; j<10-len; j++) uart_putc(' ');
        
        print_str(" - ");
        print_str(COLOR_WHITE);
        print_str(commands[i].description);
        print_str("\n");
    }
    
    print_str(COLOR_CYAN);
    print_str("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    print_str(COLOR_RESET);
    print_str("\n");
}

void cmd_clear(const char* args) {
    (void)args;
    print_str("\033[2J\033[H");
}

void cmd_echo(const char* args) {
    if (args && *args) {
        print_str(args);
        print_str("\n");
    } else {
        print_str("\n");
    }
}

void cmd_about(const char* args) {
    (void)args;
    
    print_str("\n");
    print_str(COLOR_BRIGHT_CYAN);
    print_str(STYLE_BOLD);
    print_str("╔════════════════════════════════════════════╗\n");
    print_str("║                    GTOS64                  ║\n");
    print_str("╚════════════════════════════════════════════╝\n");
    print_str(COLOR_RESET);
    print_str("\n");
    
    print_str(COLOR_YELLOW);
    print_str("  Version:      ");
    print_str(COLOR_WHITE);
    print_str("0.1.0\n");
    
    print_str(COLOR_YELLOW);
    print_str("  Architecture: ");
    print_str(COLOR_WHITE);
    print_str("AArch64 (ARM 64-bit)\n");
    
    print_str(COLOR_YELLOW);
    print_str("  Author:       ");
    print_str(COLOR_WHITE);
    print_str("Dhruv Koli\n");
    
    print_str(COLOR_YELLOW);
    print_str("  Description:  ");
    print_str(COLOR_WHITE);
    print_str("A minimal operating system\n");
    
    print_str(COLOR_RESET);
    print_str("\n");
}

void cmd_uptime(const char* args) {
    (void)args;
    
    unsigned long long cntpct, cntfrq;
    asm volatile("mrs %0, cntpct_el0" : "=r"(cntpct));
    asm volatile("mrs %0, cntfrq_el0" : "=r"(cntfrq));
    
    unsigned long long uptime_s = cntpct / cntfrq;
    unsigned int h = (unsigned int)(uptime_s / 3600);
    unsigned int m = (unsigned int)((uptime_s % 3600) / 60);
    unsigned int s = (unsigned int)(uptime_s % 60);

    print_str("\n");
    print_str(COLOR_GREEN);
    print_str("System uptime: ");
    print_str(COLOR_YELLOW);
    print_int(h);
    print_str(" hours, ");
    print_int(m);
    print_str(" minutes, ");
    print_int(s);
    print_str(" seconds\n");
    print_str(COLOR_RESET);
    print_str("\n");
}

void cmd_sysinfo(const char* args) {
    (void)args;
    extern char __kernel_start, __kernel_end;
    extern char __bss_start, __bss_end;
    
    print_str(COLOR_BRIGHT_WHITE);
    print_str(STYLE_BOLD);
    print_str("System Information:\n");
    print_str(COLOR_RESET);
    print_str(COLOR_CYAN);
    print_str("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("Architecture:      ");
    print_str(COLOR_YELLOW);
    print_str("AArch64 (ARM 64-bit)\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("Exception Level:   ");
    print_str(COLOR_YELLOW);
    print_str("EL1 (Kernel Mode)\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("Stack Pointer:     ");
    print_str(COLOR_YELLOW);
    print_ptr(STACK_TOP);
    print_str("\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("Kernel Base:       ");
    print_str(COLOR_YELLOW);
    print_ptr((void*)&__kernel_start);
    print_str("\n");
    print_str(COLOR_RESET);

    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("Kernel End:        ");
    print_str(COLOR_YELLOW);
    print_ptr((void*)&__kernel_end);
    print_str("\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("BSS Start:         ");
    print_str(COLOR_YELLOW);
    print_ptr((void*)&__bss_start);
    print_str("\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("BSS End:           ");
    print_str(COLOR_YELLOW);
    print_ptr((void*)&__bss_end);
    print_str("\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("BSS Size:          ");
    print_str(COLOR_YELLOW);
    print_int((int)((unsigned long)&__bss_end - (unsigned long)&__bss_start));
    print_str(" bytes\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("UART:              ");
    print_str(COLOR_YELLOW);
    print_str("PL011 @ ");
    print_ptr((void*)0x09000000);
    print_str("\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_CYAN);
    print_str("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    print_str(COLOR_RESET);
    print_str("\n");
}

void cmd_history(const char* args) {
    (void)args;
    print_str(COLOR_BRIGHT_WHITE);
    print_str("Command History:\n");
    print_str(COLOR_RESET);
    
    for (int i = 0; i < history_count; i++) {
        print_str(COLOR_YELLOW);
        print_str("  ");
        print_int(i + 1);
        print_str(": ");
        print_str(COLOR_WHITE);
        print_str(history_get(i));
        print_str("\n");
    }
    print_str("\n");
}

void cmd_meminfo(const char* args) {
    (void)args;
    extern char __text_start, __text_end;
    extern char __rodata_start, __rodata_end;
    extern char __data_start, __data_end;
    extern char __bss_start, __bss_end;
    extern char __kernel_end;
    
    print_str(COLOR_BRIGHT_CYAN);
    print_str("Memory Layout:\n");
    print_str(COLOR_RESET);
    print_str("  .text:         ");
    print_ptr(&__text_start);
    print_str(" - ");
    print_ptr(&__text_end);
    print_str("\n");
    print_str("  .rodata:       ");
    print_ptr(&__rodata_start);
    print_str(" - ");
    print_ptr(&__rodata_end);
    print_str("\n");
    print_str("  .data:         ");
    print_ptr(&__data_start);
    print_str(" - ");
    print_ptr(&__data_end);
    print_str("\n");
    print_str("  .bss:          ");
    print_ptr(&__bss_start);
    print_str(" - ");
    print_ptr(&__bss_end);
    print_str("\n");
    print_str("  Kernel Image:  ");
    print_ptr(&__text_start);
    print_str(" - ");
    print_ptr(&__kernel_end);
    print_str("\n");
    print_str("  Stack Top:     ");
    print_ptr(STACK_TOP);
    print_str("\n");
}

void cmd_ascii(const char* args) {
    (void)args;
    print_str(COLOR_BRIGHT_MAGENTA);
    print_str("   /\\_/\\  \n");
    print_str("  ( o.o ) \n");
    print_str("   > ^ <  \n");
    print_str(COLOR_RESET);
}

void parse_command(char* input, char** cmd, char** args) {
    while (*input == ' ') {
        input++;
    }
    
    *cmd = input;
    
    while (*input && *input != ' ') {
        input++;
    }
    
    if (*input) {
        *input = '\0';
        input++;
        
        while (*input == ' ') {
            input++;
        }
        
        *args = (*input) ? input : 0;
    } else {
        *args = 0;
    }
}

void execute_command(const char* cmd, const char* args) {
    if (!cmd || !*cmd) {
        return;
    }
    
    for (int i = 0; i < num_commands; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            commands[i].handler(args);
            return;
        }
    }
    
    print_str(COLOR_BRIGHT_RED);
    print_str("Unknown command: ");
    print_str(COLOR_YELLOW);
    print_str(cmd);
    print_str("\n");
    print_str(COLOR_WHITE);
    print_str("Type '");
    print_str(COLOR_BRIGHT_GREEN);
    print_str("help");
    print_str(COLOR_WHITE);
    print_str("' for available commands\n");
    print_str(COLOR_RESET);
}

void shell_init(void) {
    print_str(COLOR_BRIGHT_WHITE);
    print_str("Welcome to GTOS Shell!\n");
    print_str(COLOR_WHITE);
    print_str("Type '");
    print_str(COLOR_BRIGHT_GREEN);
    print_str("help");
    print_str(COLOR_WHITE);
    print_str("' to see available commands.\n\n");
    print_str(COLOR_RESET);
}

void shell_run(void) {
    char input_buffer[INPUT_BUFFER_SIZE];
    char* cmd;
    char* args;
    
    while (1) {
        print_str(shell_prompt);
        
        readline(input_buffer, sizeof(input_buffer), shell_prompt, history_count,
                 history_get, shell_completer);
        
        if (input_buffer[0] != '\0') {
            history_push(input_buffer);
        }
        
        parse_command(input_buffer, &cmd, &args);
        execute_command(cmd, args);
    }
}

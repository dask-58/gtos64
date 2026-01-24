#include "../include/shell.h"
#include "../include/input.h"
#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/string.h"

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

static const command_t commands[] = {
    {"help", "Show available commands", cmd_help},
    {"clear", "Clear the screen", cmd_clear},
    {"echo", "Print text to screen", cmd_echo},
    {"about", "About this operating system", cmd_about},
    {"uptime", "Show system uptime", cmd_uptime},
    {"sysinfo", "Display system information", cmd_sysinfo},
};

static const int num_commands = sizeof(commands) / sizeof(command_t);

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
    print_ptr((void*)0x40400000);
    print_str("\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_GREEN);
    print_str("  [✓] ");
    print_str(COLOR_WHITE);
    print_str("Kernel Base:       ");
    print_str(COLOR_YELLOW);
    print_ptr((void*)0x40000000);
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
    char input_buffer[128];
    char* cmd;
    char* args;
    
    while (1) {
        print_str(COLOR_BRIGHT_CYAN);
        print_str("gtos");
        print_str(COLOR_WHITE);
        print_str("@");
        print_str(COLOR_BRIGHT_YELLOW);
        print_str("kernel");
        print_str(COLOR_BRIGHT_WHITE);
        print_str(" $ ");
        print_str(COLOR_RESET);
        
        readline(input_buffer, sizeof(input_buffer));
        
        parse_command(input_buffer, &cmd, &args);
        execute_command(cmd, args);
    }
}
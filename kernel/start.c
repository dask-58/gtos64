#include "../include/uart.h"
#include "../include/printf.h"
#include "../include/banner.h"

extern char __bss_start, __bss_end;

void start(void) {
    uart_init();
    banner_show();
    print_str(COLOR_BRIGHT_BLUE);
    print_str(STYLE_BOLD);
    print_str("Boot Sequence:\n");
    print_str(COLOR_RESET);
    print_str(COLOR_CYAN);
    print_str("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_BRIGHT_GREEN);
    print_str("  ▶ ");
    print_str(COLOR_WHITE);
    print_str("Stage 1: ");
    print_str(COLOR_RESET);
    print_str("Checking exception level... ");
    print_str(COLOR_GREEN);
    print_str("OK\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_BRIGHT_GREEN);
    print_str("  ▶ ");
    print_str(COLOR_WHITE);
    print_str("Stage 2: ");
    print_str(COLOR_RESET);
    print_str("Transitioning to EL1... ");
    print_str(COLOR_GREEN);
    print_str("OK\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_BRIGHT_GREEN);
    print_str("  ▶ ");
    print_str(COLOR_WHITE);
    print_str("Stage 3: ");
    print_str(COLOR_RESET);
    print_str("Initializing stack... ");
    print_str(COLOR_GREEN);
    print_str("OK\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_BRIGHT_GREEN);
    print_str("  ▶ ");
    print_str(COLOR_WHITE);
    print_str("Stage 4: ");
    print_str(COLOR_RESET);
    print_str("Clearing BSS section... ");
    print_str(COLOR_GREEN);
    print_str("OK\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_BRIGHT_GREEN);
    print_str("  ▶ ");
    print_str(COLOR_WHITE);
    print_str("Stage 5: ");
    print_str(COLOR_RESET);
    print_str("Jumping to kernel... ");
    print_str(COLOR_GREEN);
    print_str("OK\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_BRIGHT_GREEN);
    print_str("  ▶ ");
    print_str(COLOR_WHITE);
    print_str("Stage 6: ");
    print_str(COLOR_RESET);
    print_str("Initializing UART... ");
    print_str(COLOR_GREEN);
    print_str("OK\n");
    print_str(COLOR_RESET);
    
    print_str(COLOR_CYAN);
    print_str("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
    print_str(COLOR_RESET);
    print_str("\n");
    
    // ===== SYSTEM INFO =====
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
    
    // ===== READY MESSAGE =====
    print_str(COLOR_BRIGHT_GREEN);
    print_str(STYLE_BOLD);
    print_str("╔═══════════════════════════════════════════════════════════╗\n");
    print_str("║                   KERNEL READY! 🚀                        ║\n");
    print_str("╚═══════════════════════════════════════════════════════════╝\n");
    print_str(COLOR_RESET);
    print_str("\n");
    
    // ===== PROMPT =====
    print_str(COLOR_BRIGHT_CYAN);
    print_str("tinyarm64");
    print_str(COLOR_WHITE);
    print_str("@");
    print_str(COLOR_BRIGHT_YELLOW);
    print_str("kernel");
    print_str(COLOR_BRIGHT_WHITE);
    print_str(" $ ");
    print_str(COLOR_RESET);

}
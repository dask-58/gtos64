#ifndef SHELL_H
#define SHELL_H

void shell_init(void);
void shell_run(void);
void parse_command(char *input, char **cmd, char **args);
void execute_command(const char *cmd, const char *args);

#endif
#ifndef INPUT_H
#define INPUT_H

typedef const char *(*history_getter_t)(int index);
typedef void (*completion_callback_t)(char *buffer, int *length, int *cursor,
                                      int max_len);

void readline(char *buffer, int max_length, const char *prompt,
              int history_count, history_getter_t history_getter,
              completion_callback_t completer);

#endif

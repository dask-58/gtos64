#ifndef INPUT_H
#define INPUT_H

typedef void (*completion_callback_t)(char *buffer, int *index, int max_len);

void readline(char *buffer, int max_length, char (*history)[128],
              int history_count, completion_callback_t completer);

#endif
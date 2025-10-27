#ifndef ERROR_H
#define ERROR_H

#include <sep_stack.h>
#include <sep_string.h>


typedef struct {
    const char *function;
    int         line_number;
    const char *module;
    const char* message;
}Error_Element;


typedef struct {
    Stack *stacktrace;

}ErrorState;

void init_error_state(ErrorState *error_state);
void add_to_stacktrace(ErrorState *error_state, const char *message, const char *function, int line_number, const char *module);
void print_stacktrace(ErrorState *error_state);

#endif

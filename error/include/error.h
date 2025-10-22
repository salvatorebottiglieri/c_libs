#ifndef ERROR_H
#define ERROR_H

#include <sep_string.h>




void add_to_stacktrace(ErrorState *error_state, Error_Element *element);
void print_stacktrace(ErrorState *error_state);
void free_stacktrace(ErrorState *error_state);



#endif

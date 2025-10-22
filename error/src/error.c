#include "../include/error.h"

#include <stack.h>


typedef struct {
    String *function;
    String *line_number;
    String *module;
}Error_Element;


typedef struct {
    String *message;
    Stack *stacktrace;

}ErrorState;

void add_to_stacktrace(ErrorState *error_state, Error_Element *element){


}
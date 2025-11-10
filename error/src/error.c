#include "../include/sep_error.h"
#include <stdio.h>





void init_error_state(ErrorState *error_state){
    error_state->stacktrace = create_stack(sizeof(Error_Element));
}

void add_to_stacktrace(ErrorState *error_state, const char *message, const char *function, int line_number, const char *module){
    if (error_state->stacktrace == NULL) return;
    Error_Element *error_element = (Error_Element*) malloc(sizeof(Error_Element));
    if (error_element == NULL) return;
    error_element->function = function;
    error_element->line_number = line_number;
    error_element->module = module;
    error_element->message = message;
    push_stack(error_state->stacktrace, error_element);
}



void print_error_element(void* element){
    Error_Element *error_element = (Error_Element*) element;
    printf("SONO QUI ");
    printf("%s:%s:%d\n", error_element->module, error_element->function, error_element->line_number);

}

void print_stacktrace(ErrorState *error_state){
    if (error_state->stacktrace == NULL) return;

    print_stack(error_state->stacktrace, print_error_element);
}



ErrorState error_state = {
    .stacktrace = NULL
};





int f3(void){
    printf("f3\n");
    if ( 1 == 1){
        add_to_stacktrace(&error_state, "error in f3",__func__,__LINE__, __FILE__);
        return -1;
    }
    return 0;

}

int f2(void){
    printf("f2\n");
    if (f3() == -1){
        add_to_stacktrace(&error_state, "error in f2",__func__,__LINE__, __FILE__);
        return -1;
    }
    return 0;
}

int f1(void){
    printf("f1\n");
    if (f2() == -1){
        add_to_stacktrace(&error_state, "error in f2",__func__,__LINE__, __FILE__);
        return -1;
    }

    return 0;
}   


int main(void){

    init_error_state(&error_state);
    if (f1() != 0){
        print_stacktrace(&error_state);
    }


    return 0;
}
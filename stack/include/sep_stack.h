#ifndef STACK_H
#define STACK_H


#include <stdlib.h>

typedef void (*print_stack_elem)(void*);

/* The stack is agnostic to the type of data it holds, so it uses a set of pointers
    to the data. The structure doesn't mantain a copy of the data but only a reference to it
    so be careful when using it and free the data only when the stack is deleted */
typedef struct {
    void** data;
    size_t elem_size;
    size_t size;
    size_t capacity;
    void**  head;
}Stack;



Stack* create_stack();
void push_stack(Stack* stack, void* data);
void* top_stack(Stack* stack);
void* pop_stack(Stack* stack);
void free_stack(Stack* stack);
void print_stack(Stack* stack,print_stack_elem print);


#endif

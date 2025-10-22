#ifndef STACK_H
#define STACK_H


#include <stdlib.h>


typedef struct {
    void** data;
    size_t elem_size;
    size_t size;
    size_t capacity;
    void*  head;
}Stack;


Stack* create_stack(size_t elem_size);
void push_stack(Stack* stack, void* data);
void* top_stack(Stack* stack);
void* pop_stack(Stack* stack);
void free_stack(Stack* stack);


#endif

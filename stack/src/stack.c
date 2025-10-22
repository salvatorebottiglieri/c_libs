#include "../include/sep_stack.h"

#include <string.h>

Stack* create_stack(size_t elem_size){

    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    stack->data = malloc(elem_size);
    stack->elem_size = elem_size;
    stack->size = 0;
    stack->capacity = 1;
    stack->head = NULL;
    return stack;
}


void free_stack(Stack* stack){
    if (stack != NULL){
        for (size_t i = 0; i < stack->size; i++){
            free(stack->data[i]);
        }
    }
    free(stack);
    stack = NULL;
}

void push_stack(Stack* stack, void* data){
    if (stack == NULL) return;
    if (stack->size == stack->capacity){
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->capacity*stack->elem_size);
        if (stack->data == NULL) return;
    }
    stack->head += stack->elem_size;
    memcpy(stack->head, data, stack->elem_size);
    stack->size += 1;
}

void* top_stack(Stack* stack){
    if (stack == NULL) return NULL;
    return stack->head;
}

void* pop_stack(Stack* stack){
    if (stack == NULL) return NULL;
    void* data = stack->head;
    stack->head -= stack->elem_size;
    stack->size -= 1;
    return data;
}

#include "../include/sep_stack.h"

#include <string.h>

#include <stdio.h>

#define ptr_to_ptr_size sizeof(void**)

Stack* create_stack(){

    Stack* stack = malloc(sizeof(Stack));
    if (stack == NULL) return NULL;
    stack->data = malloc(ptr_to_ptr_size);
    if (stack->data == NULL) return NULL;
    stack->elem_size = ptr_to_ptr_size;
    stack->size = 0;
    stack->capacity = 1;
    stack->head = stack->data;
    return stack;
}


void free_stack(Stack* stack){
    if (stack != NULL){
        free(stack->data);
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

    if (stack->size != 0)stack->head += stack->elem_size;
    memcpy(stack->head, &data, stack->elem_size);


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

void print_stack(Stack* stack, print_stack_elem print){
    if (stack == NULL) return;
    void** current_elem = stack->head;

    while(current_elem >= stack->data){

        printf("------------------------------------\n");
        print(*current_elem);
        printf("------------------------------------\n");
        if (current_elem != stack->data)printf("                 |                  \n");
        current_elem -= stack->elem_size;
    }
}

struct test{
    int* one;
    float *two;
};

void print_test(void* data){

    struct test t = *(struct test*)data;
    printf("{\n");
    printf("one: %d\n", *(int*)t.one);
    printf("two: %f\n", *(float*)t.two);
    printf("}\n");

}

//only for debugging
int main(){
    struct test *some_ptr;
    struct test t ={
        .one = malloc(sizeof(int*)),
        .two = malloc(sizeof(float*))
    };
    int one = 1;
    float two = 2.0;
    t.one = &one;
    t.two = &two;
    
    Stack* stack = create_stack();

    some_ptr = &t;
    push_stack(stack, some_ptr);

    push_stack(stack, some_ptr);
    push_stack(stack, some_ptr);
    push_stack(stack, some_ptr);
    push_stack(stack, some_ptr);

    print_stack(stack, print_test);
    free_stack(stack);
    return 0;

}

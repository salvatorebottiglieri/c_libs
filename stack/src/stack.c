#include "../include/sep_stack.h"

#include <string.h>

#include <stdio.h>

#define ptr_to_ptr_size sizeof(void**)

/**
 * Creates a new Stack object with the given initial capacity.
 *
 * @return A newly created Stack object with the given initial capacity,
 *         or NULL if the allocation failed.
 */
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


/**
 * Frees all memory allocated by the given Stack object and sets it to NULL.
 * @param stack The Stack object to free
 */
void free_stack(Stack* stack){
    if (stack != NULL){
        free(stack->data);
    }
    free(stack);
    stack = NULL;
}

/**
 * Pushes a new element onto the given Stack object.
 * If the Stack is at maximum capacity, it will be resized to double its capacity.
 * The new element will be copied into the Stack.
 * @param stack The Stack object to push onto
 * @param data The element to push onto the Stack
 */
void push_stack(Stack* stack, void* data){
    if (stack == NULL) return;
    
    if (stack->size == stack->capacity){
        stack->capacity *= 2;
        stack->data = realloc(stack->data, stack->capacity*stack->elem_size);
        if (stack->data == NULL) return;

    }
    // head points to the next free memory location
    if (stack->size != 0)stack->head = ((size_t) stack->head) + stack->elem_size;
    

    
    memcpy(stack->head, &data, stack->elem_size);
    stack->size += 1;
}

/**
 * Returns the element at the top of the given Stack object, or NULL if the Stack is empty.
 * The returned element is a pointer to the actual element stored in the Stack, so
 * it should be cast to the correct type before use.
 * @param stack The Stack object to retrieve the top element from
 * @return The element at the top of the Stack, or NULL if the Stack is empty
 */
void* top_stack(Stack* stack){
    if (stack == NULL) return NULL;
    return stack->head;
}

/**
 * Pops an element from the top of the given Stack object and returns it.
 * If the Stack is empty, NULL is returned.
 * The returned element is a pointer to the actual element stored in the Stack, so
 * it should be cast to the correct type before use.
 * @param stack The Stack object to pop from
 * @return The popped element, or NULL if the Stack is empty
 */
void* pop_stack(Stack* stack){
    if (stack == NULL) return NULL;
    void* data = stack->head;
    stack->head -= stack->elem_size;
    stack->size -= 1;
    return data;
}

/**
 * Prints the elements of the given Stack object in order from top to bottom.
 * Each element is passed to the given print_stack_elem function, which should
 * print the element in a human-readable format.
 * @param stack The Stack object to print
 * @param print The function to use to print each element of the Stack
 */
void print_stack(Stack* stack, print_stack_elem print){
    if (stack == NULL) return;
    void** current_elem = stack->head;

    while(current_elem >= stack->data){

        printf("------------------------------------\n");
        print(*current_elem);
        printf("------------------------------------\n");
        if (current_elem != stack->data)printf("                 |                  \n");
        current_elem = ((size_t)current_elem) - stack->elem_size;
    }
    printf("\n");
}

    struct test{
        int a;
        char b;
    };

void print_test_element(void* element){
    struct test* t = (struct test*) element;
    printf("a: %d, b: %c\n", t->a, t->b);
}

//only for debugging
int main(){



    struct test t1 = {1, 'a'};
    struct test t2 = {2, 'b'};

    Stack* stack = create_stack();
    push_stack(stack, &t1);
    push_stack(stack, &t2);
    print_stack(stack, print_test_element);
    free_stack(stack);


    return 0;

}

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
int push_stack(Stack* stack, void* data){
    if (stack == NULL) return -1;
    void** new_address=NULL;

    if (stack->size == stack->capacity){
        stack->capacity *= 2;
        new_address = realloc(stack->data, stack->capacity*stack->elem_size);
        if (new_address == NULL) return -1;
        if (new_address != stack->data){
            stack->data = new_address;
            //put head back to the address of the last item of the stack
            stack->head = ((size_t) new_address) + ((stack->size-1) * stack->elem_size);

        }
        

    }
    // head points to the next free memory location to accept the new adding element
    if (stack->size != 0)stack->head = ((size_t) stack->head) + stack->elem_size;
    
    memcpy(stack->head, &data, stack->elem_size);
    stack->size += 1;
    return 0;
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


typedef struct {
    const char *function;
    int         line_number;
    const char *module;
    const char* message;
}Error_Element;


typedef struct {
    Stack *stacktrace;

}ErrorState;


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

ErrorState error_state = {
    .stacktrace = NULL
};

void print_error_element(void* element){
    Error_Element *error_element = (Error_Element*) element;
    printf("%s:%s:%d\n", error_element->module, error_element->function, error_element->line_number);

}

void print_stacktrace(ErrorState *error_state){
    if (error_state->stacktrace == NULL) return;

    print_stack(error_state->stacktrace, print_error_element);
}

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
        add_to_stacktrace(&error_state, "",__func__,__LINE__, __FILE__);
        return -1;
    }
    return 0;
}

int f1(void){
    printf("f1\n");
    if (f2() == -1){
        add_to_stacktrace(&error_state, "",__func__,__LINE__, __FILE__);
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
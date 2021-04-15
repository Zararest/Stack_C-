#include "stack.inl"
#include <stdio.h>

int main(){
    int var;

    Stack<int> my_stack(10, 10); 

    
    my_stack.push(10);
    my_stack.push(20);
    my_stack.push(30);

    printf("stack : %i\n", my_stack.pop());
    
    printf("stack : %i\n", my_stack.pop());
    printf("stack : %i\n", my_stack.pop());
    return 0; // echo $?
}
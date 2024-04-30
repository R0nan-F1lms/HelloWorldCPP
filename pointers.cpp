#include <stdio.h>

int main() {
    int num = 10; // STORE int num [10]
    int *ptr;     // Declare the pointer ptr can hold value of int
    
    ptr = &num;   // pointing to num
    
    // Print the value and address of 'num'
    printf("Value of num: %d\n", num);
    printf("Address of num: %p\n", (void*)&num);
    
    // Print the value and address using the pointer
    printf("Value using pointer: %d\n", *ptr);
    printf("Address using pointer: %p\n", (void*)ptr);
    
    return 0;
}

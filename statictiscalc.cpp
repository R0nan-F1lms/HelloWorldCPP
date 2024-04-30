/* 
Const: MAX_NUMBERS int with value 20

Struct: Number Data
Fields:
- Values: array of MAX_NUMBERS double values
- Size: an integer for the current number of values


*/
#include <stdio.h>
#include "splashkit.h"

// Define a constant for the maximum number of values
#define MAX_NUMBERS 20

// Define a struct to store number data
struct NumberData{
    double values[MAX_NUMBERS];
    int size;
};

// Function to read a double value from the user
double read_double() {
    double num;
    printf("Enter a number: ");
    num = convert_to_double(read_line());
    return num;
}

// Procedure to populate the array with values entered by the user
void populate_array(NumberData *data) {
    printf("Enter the number of values you want to enter (up to %d): ", MAX_NUMBERS);
    data->size = convert_to_integer(read_line());
    
    // Limit the number of values to the size of the array
    if (data->size > MAX_NUMBERS) {
        printf("Maximum number of values exceeded. Only %d values will be accepted.\n", MAX_NUMBERS);
        data->size = MAX_NUMBERS; 
        /* 
        IF you enter a number higher than the max it will give you the max 
        number you can have in the array which is 20. 
        */
    }
    
    // Loop to read each value and store it in the array
    for (int i = 0; i < data->size; ++i) {
        data->values[i] = read_double();
    }
}

// Function to calculate the sum of values in the array
double calculate_sum(const NumberData *data) {
    double sum = 0.0;
    for (int i = 0; i < data->size; ++i) {
        sum += data->values[i];
    }
    return sum;
}

// Function to calculate the mean of values in the array
double calculate_mean(const NumberData *data) {
    double sum = calculate_sum(data);
    return sum / data->size;
}

// Function to find the maximum value in the array
double find_maximum(const NumberData *data) {
    double max = data->values[0];
    for (int i = 1; i < data->size; ++i) {
        if (data->values[i] > max) {
            max = data->values[i];
        }
    }
    return max;
}

// Main function to orchestrate the program
int main() {
    NumberData data;
    populate_array(&data);
    
    printf("Statistics:\n");
    printf("Sum:\t\t%.2f\n", calculate_sum(&data));
    printf("Mean:\t\t%.2f\n", calculate_mean(&data));
    printf("Maximum:\t%.2f\n", find_maximum(&data));
    
    return 0;
}
#include <cstdio>

void swap(int &x, int &y) {
    int temp = x;
    x = y;
    y = temp;
}

int main() {
    int a = 5;
    int b = 10;

    printf("Before swapping: a = %d, b = %d\n", a, b);

    swap(a, b);

    printf("After swapping: a = %d, b = %d\n", a, b);

    return 0;
}

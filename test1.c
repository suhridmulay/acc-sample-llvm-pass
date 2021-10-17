#include <stdio.h>

long fibbonaci(int n) {
    if (n <= 1) { return n; } 
    else { return fibbonaci(n - 1) + fibbonaci(n - 2); }
}

int main() {
    for (int i = 0; i < 10; i++) { printf("%ld ", fibbonaci(i)); }
    printf("\n");
}
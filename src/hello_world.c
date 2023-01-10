#include <stdio.h>

void print_hi_n_times(int n) {
    for (int i = 0; i < n; i++) {
        puts("Hello World!");
    }
}

int main(int argc, char** argv) {
    print_hi_n_times(6);
    return 0;
}


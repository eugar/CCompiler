#include <stdio.h>

int main(int argc, char const *argv[]) {
    int i = 2;
    double d = 2.4;
    char c = 'c';

    if (c > 1 && i >= 2 || !(i <= 2) || i == c){
        c++;
    }
    i += 1;
    i / 2;

    printf("Hello World\n");
    return 0;
}

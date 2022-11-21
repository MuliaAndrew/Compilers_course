#include <stdio.h>


int main () {
    int x;
    scanf("%d", &x);
    int res = 1; 
    for (int i = 1; i <= x; i++)
        res *= i;
    printf("%d\n", res);
    return 0;
}
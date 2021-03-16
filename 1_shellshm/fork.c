#include <stdio.h>
#include <unistd.h>

int main() {
    int i = 1;
    if (fork() == 0) {
        i++;
        if(fork() == 0)
            i = 4;
        else {
            ++i;
            fork();
        }
    } else{
        i = 7;
        if (fork() != 0)
            ++i;
    }

    if (i > 5) {
        fork();
        i = i + 2;
    }
    printf("%d\n", i);
    return 0;
}
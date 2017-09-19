#include <unistd.h>
#include <stdio.h>

// 执行： gcc fork.c -Wall -o fork.out && sudo ./fork.out
int main() {
    pid_t fpid;
    fpid = fork();
    if (fpid < 0) {
        printf("error in fork");
    } else if (fpid == 0) {
        printf("\nI am child. Process id is %d\n", getpid());
    } else {
        printf("\nI am parent. Process id is %d\n", getpid());
    }
    return 0;
}
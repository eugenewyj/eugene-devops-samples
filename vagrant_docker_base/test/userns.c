#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/capability.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024*1024)

static char child_stack[STACK_SIZE];
char* const child_args[] = {
    "/bin/bash",
    NULL
};

void set_uid_map(pid_t pid, int inside_id, int outside_id, int length) {
    printf("父进程ID=%d\n", pid);
    printf("当前进程ID=%d\n", getpid());
    char path[256];
    sprintf(path, "/proc/%d/uid_map", getpid());
    FILE *uid_map = fopen(path, "w");
    fprintf(uid_map, "%d %d %d", inside_id, outside_id, length);
    fclose(uid_map);
}

void set_gid_map(pid_t pid, int inside_id, int outside_id, int length) {
    printf("父进程ID=%d\n", pid);
    printf("当前进程ID=%d\n", getpid());
    char path[256];
    sprintf(path, "/proc/%d/gid_map", getpid());
    FILE *gid_map = fopen(path, "w");
    fprintf(gid_map, "%d %d %d", inside_id, outside_id, length);
    fclose(gid_map);
}

int child_main(void* args) {
    printf("在子进程=%d 中！\n", getpid());
    set_uid_map(getpid(), 0, 1000, 1);
    set_gid_map(getpid(), 0, 1000, 1);
    printf("eUID = %ld; eGID = %ld; ", (long)geteuid(), (long)getegid());
    cap_t caps;
    caps = cap_get_proc();
    printf("capabilities: %s\n", cap_to_text(caps, NULL));
    execv(child_args[0], child_args);
    return 1;
}

// 执行： gcc userns.c -Wall -lcap -o userns.out && sudo ./userns.out
int main(){
    printf("程序开始=%d：\n", getpid());
    int child_pid = clone(child_main, child_stack + STACK_SIZE, CLONE_NEWUSER | SIGCHLD, NULL);
    waitpid(child_pid, NULL, 0);
    printf("已退出\n");
    return 0;
}
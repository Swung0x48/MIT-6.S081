#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    int parent_to_child[2]; // parent writes, child reads
    int child_to_parent[2]; // parent reads, child writes
    pipe(parent_to_child);
    pipe(child_to_parent);
    int pid = fork();
    if (pid == 0){
        // child
        char c;
        read(parent_to_child[0], &c, sizeof(char));
        printf("%d: received ping\n", getpid());
        write(child_to_parent[1], &c, sizeof(char));
        exit(0);
    } else {
        // parent
        write(parent_to_child[1], '\0', sizeof(char));
        char a;
        read(child_to_parent[0], &a, sizeof(char));
        printf("%d: received pong\n", getpid());
        exit(0);
    }
}

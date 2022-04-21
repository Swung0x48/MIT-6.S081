#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int prime_sieve(int read_fd) {
    int prime;
    if (!read(read_fd, &prime, sizeof(prime))) {
        close(read_fd);
        exit(1);
        return 1;
    }
    printf("prime %d\n", prime);
    int p[2];
    pipe(p);

    int pid = fork();
    int n;
    if (pid == 0) {
        while (read(read_fd, &n, sizeof(n)) == sizeof(int)) {
            if (n % prime != 0) {
                write(p[1], &n, sizeof(n));
            }
        }
        close(read_fd); // close left-read side
        close(p[1]);    // close right-write side
        p[1] = -1;      // invalidate fd
        close(p[0]);
        p[0] = -1;
        wait((int*) 0);
        exit(0);
    } else {
        close(p[1]);    // close right-write side in child process (forked before close)
        p[1] = -1;      // invalidate fd
        int ret = prime_sieve(p[0]);
        close(p[0]);
        exit(ret);
    }
    return 0;
}

int
main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    int pid = fork();
    if (pid == 0) {
        // parent
        for (int i = 2; i <= 35; ++i) {
            write(p[1], &i, sizeof(int));
        }
        close(p[1]);
        p[1] = -1;
        close(p[0]);
        p[0] = -1;
        wait((int*) 0);
        exit(0);
    } else {
        // child
        close(p[1]);
        p[1] = -1;
        int ret = prime_sieve(p[0]);
        close(p[0]);
        p[0] = -1;
        exit(ret);
    }
}


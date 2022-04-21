#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int exec_with_fork(char* file, char** argv) {
    int pid = fork();
    if (pid == 0) {
        return 0;
    } else {
        exec(file, argv);
        fprintf(2, "exec %d failed", file);
    }
    return -1;
}

char args[MAXARG][64] = {0};

int
main(int argc, char *argv[]) {
    char* file = argv[1];
    int init_arg_count = argc - 1;
    int arg_count = init_arg_count;
    for (int i = 1; i < argc; ++i) {
        strcpy(args[i - 1], argv[i]); // copy arguments right after xargs, i.e. xargs arg0 args1 ...
    }                                 //                                             [^^^^^^^^^^^^^]

    char* new_argv[MAXARG];
    int arg_ch_count = 0;
    char ch;
    while (read(0, &ch, sizeof(char)) == sizeof(char)) {
        if (ch == '\n') {
            args[arg_count][arg_ch_count++] = 0;
            ++arg_count;
            for (int i = 0; i < arg_count; ++i) {
                new_argv[i] = args[i];
            }
            exec_with_fork(file, new_argv);
            arg_count = init_arg_count;
        } else if (ch == ' ') {
            args[arg_count][arg_ch_count++] = 0;
            ++arg_count;
            arg_ch_count = 0;
        } else {
            args[arg_count][arg_ch_count++] = ch;
        }
        
    }
    exit(0);
}

// main(argc, [cmd, arg0, arg1, ...])
// exec(cmd, [cmd, arg0, arg1, ...])
// echo prints "arg0 arg1 ..."
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(2, "Usage: sleep time\n");
        exit(1);
    }
    char* time_str = argv[1];
    int time = atoi(time_str);
    sleep(time);
    exit(0);
}

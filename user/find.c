#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char* dir, char* objective) {
    static char buf[512];
    if (strcmp(dir, "..") == 0)
        return 0;

    int fd;
    if((fd = open(dir, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", dir);
        return 1;
    }
    struct stat st;
    if(fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", dir);
        close(fd);
        return 1;
    }
    switch (st.type) {
        case T_FILE:
            int last_backslash = 0;
            for (int i = 0; dir[i] != '\0'; ++i) {
                if (dir[i] == '/')
                    last_backslash = i;
            }
            if (strcmp(&dir[last_backslash + 1], objective) == 0)
                printf("%s\n", dir);
            break;
        case T_DIR:
            if (strlen(dir) + 1 + DIRSIZ + 1 > sizeof(buf)){
                fprintf(2, "ls: path too long\n");
                break;
            }
            strcpy(buf, dir);
            char* p = buf + strlen(buf);
            *p = '/';
            p++;
            struct dirent de;
            while(read(fd, &de, sizeof(de)) == sizeof(de)) {
                if (de.inum == 0 || de.name[0] == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
                    continue;
                memmove(p, de.name, DIRSIZ);
                p[DIRSIZ] = 0;
                find(buf, objective);
            }
            break;
    }
    close(fd);
    return 0;
}

int
main(int argc, char *argv[])
{
    if (argc <= 2) {
        printf("Usage: find dir file\n", argc);
        exit(1);
    }

    exit(find(argv[1], argv[2]));
}
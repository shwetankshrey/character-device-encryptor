#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int f, res;
    f = open("/dev/encrypto", O_RDONLY);
    if(f < 0) {
        printf("not found sad\n");
        return;
    }
    res = write(f, "no", strlen("no"));
    res = write(f, "no", strlen("no"));
    res = write(f, "no", strlen("no"));
    printf("Halfway\n");
    close(f);
    f = open("/dev/encrypto", O_RDONLY);
    if(f < 0) {
        printf("not found sad\n");
        return;
    }
    res = write(f, "no", strlen("no"));
    res = write(f, "no", strlen("no"));
    close(f);
}
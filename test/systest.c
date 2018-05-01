#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main() {
    int f1, f2, res;
    char *str = "hello world ";
    f1 = open("/dev/encrypto", O_RDWR);
    res = write(f1, str, strlen(str));
    char c1[256];
    res = read(f1, c1, 256);
    unsigned char d1[256];
    int i;
    for(i = 0 ; i < 256 ; i++) {
        d1[i] = (unsigned char) c1[i];
        // printf("%02x", d1[i]);
    }
    res = close(f1);
    // printf("\n");
    f2 = open("/dev/decrypto", O_RDWR);
    res = write(f2, (char *) d1, 256);
    char c2[256];
    res = read(f2, c2, 256);
    unsigned char d2[256];
    for(i = 0 ; i < 256 ; i++) {
        d2[i] = (unsigned char) c2[i];
        // printf("%02x", d2[i]);
    }
    printf("%s\n", d2);
    res = close(f2);
}
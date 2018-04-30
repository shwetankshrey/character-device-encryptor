#include <stdio.h>
#include <fcntl.h>

int main() {
    int f = open("/dev/encrypto", O_RDONLY);
    char myRandomData[50];
    ssize_t result = read(f, myRandomData, sizeof(myRandomData));
}
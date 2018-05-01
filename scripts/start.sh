#!/bin/sh

cd ..
cd src
make
insmod encrypto.ko
insmod decrypto.ko
chmod 777 /dev/encrypto
chmod 777 /dev/decrypto
cd ..
cd test
gcc systest.c
./a.out


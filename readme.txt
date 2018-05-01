OS Assignment 4

Shwetank Shrey  2016095
Rohan Chhokra   2016080


Directory Structure :
        .
        |-- readme.txt
        |-- scripts
        |   |-- end.sh
        |   `-- start.sh
        |-- src
        |   |-- decrypto.c
        |   |-- encrypto.c
        |   `-- Makefile
        `-- test
            `-- systest.c


Code Description :

Modules are kernel programs that do not have to be compiled with the entire kernel. Every module consists of an init and an exit point, where we specify the functions to be called at that time. Our entry point for both character devices is the device registration step, in which a major number is assigned to the device and it is registered. Similarly, the unregister device simply unregisters the device. In the registration step, we also associate the file operations to the character devices using the file_operations struct. We have written the open, read, write and release operations. The open operation simply enables us to access the character device via its location on the VFS. Similarly the release operation deallocates global memory spaces. The major code for the encryptor and decryptor is written in the write as well as read operations. We use the given algorithm and write the corresponding code. The encryptor's write creates the encrypted file which is given to the user via the read and the decryptor's write decrypts the encrypted file into an understandable language, which is returned to the user with it's read operation.


Compilation:

Simply open the scripts folder and ./start.sh to start and ./end.sh to end.


User Input:

The user can provide his input in the systest.c file, where the character devices are called.


Expected Output:

The expected output is the input itself.

#include "stdio.h"
#include "fcntl.h"
#include <sys/types.h>
#include <sys/stat.h>
#define AES_KEY_MAX_LEN (256 >> 3)
#define AES_KEY_FILE    "aes_key.file"

void gadgets() {
    __asm("pop %ecx");
    __asm("int $0x80");

    __asm("mov $0x04, %eax");
    __asm("ret");

    __asm("mov $0x01, %ebx");
    __asm("ret");

    __asm("mov $0xffff, %edx");
    __asm("ret");

    __asm("ret");

}
void read_key(unsigned char *out, unsigned int *outlen, \
        char *filename);
void encrypt_demo(unsigned char *aes_key, int key_len);
void do_encrypt(unsigned char *aes_key,  int key_len);

int main() {
    unsigned char aes_key[AES_KEY_MAX_LEN];
    unsigned int  aes_key_len = AES_KEY_MAX_LEN;

    read_key(aes_key, &aes_key_len, AES_KEY_FILE);

    if(aes_key_len <= 0) 
        return 0;

    encrypt_demo(aes_key, aes_key_len);

    return 0;
}

void read_key(unsigned char *out, unsigned int *outlen,\
        char *filename) {
    int fd = open(filename, O_RDONLY);

    if(-1 == fd) {
        return ;
    }

    *outlen = read(fd, out, *outlen);

    return ;
}
void encrypt_demo(unsigned char *aes_key, int key_len) {
    while(1) {
        do_encrypt(aes_key, key_len);

        break;
    }
}
void do_encrypt(unsigned char *aes_key,  int key_len) {
    char buf[100]; // = "anything";
    int buflen;     // = anylen
    char *out;
    int outlen;

    gets(buf);

    buflen = strlen(buf);

    // do encrypt
}

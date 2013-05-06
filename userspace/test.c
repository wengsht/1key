/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/15/2013 08:33:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>

#include "ok_const.h"
#include "ok_api_const.h"
#include "ok_api_all.h"

int main()
{
    struct OK_CONTEXT ok_context;

    Ok_Create_Context(&ok_context);

    Ok_Print_Version(&ok_context);

    char ss[] = "/home/wengsht/1.txt";
    char ss2[] = "/home/wengsht/2.txt";

    Ok_Create_Srk(&ok_context);
    Ok_Load_Srk(&ok_context);

    Ok_Create_User_Rsa(&ok_context, ss, "123", 3);
    int out;
    int out2;
    Ok_Load_User_Rsa(&ok_context, ss, "123", 3, &out);
    Ok_Create_User_Wrap_Aes(&ok_context, out, ss2);
    Ok_Load_User_Wrap_Aes(&ok_context, out,ss2, &out2);
    printf("%p\n", (int *)out);

    int inlen = 41;
    int outlen = 1024;
    char s[1024] = "wengsht v9  256tyyyyuijkjkmnjhuiu87uijkm";
    char tmp[1024];
    Ok_Encrypt_User_Data_By_Aes(&ok_context, s, inlen, tmp, &outlen, out2);
    inlen = 1024;
    s[5] = '0';
    printf("%s\n", s);
    Ok_Decrypt_User_Data_By_Aes(&ok_context, tmp, outlen, s, &inlen, out2);

    printf("%s\n", s);
    /* 
    Ok_Create_User_Aes(&ok_context, ss);
    int out;
    Ok_Load_User_Aes(&ok_context, ss, &out);
    printf("%p\n", (int *)out);

    int inlen = 41;
    int outlen = 1024;
    char s[1024] = "wengsht v9  256tyyyyuijkjkmnjhuiu87uijkm";
    char tmp[1024];
    Ok_Encrypt_User_Data_By_Aes(&ok_context, s, inlen, tmp, &outlen, out);
    inlen = 1024;
    s[5] = '0';
    printf("%s\n", s);
    Ok_Decrypt_User_Data_By_Aes(&ok_context, tmp, outlen, s, &inlen, out);

    printf("%s\n", s);
 */
    /*  
    char s[4000] = "            wengsht v9";
    *((int *)(s+4)) = out;
    *((int *)(s+8)) = 11;
    ioctl(ok_context.fs, OK_RSA_ENCRYPT, s+4);

    *((int *)s) = out;
    ioctl(ok_context.fs, OK_RSA_DECRYPT, s);
    printf("%s\n", s+4);
    */
    /* 
    char s[] = "wengsht v9";


    int inlen = 11;
    int outlen = 1024;
    char tmp[1024];
    Ok_Encrypt_User_Data_By_Rsa(&ok_context, s, inlen, tmp, &outlen, out);
    inlen = 1024;
    s[5] = '0';
    Ok_Decrypt_User_Data_By_Rsa(&ok_context, tmp, outlen, s, &inlen, out);

    printf("%s\n", s);
    */
    /* 
    char str[] = "0engsht";
    unsigned char hash[16];
    int outlen = 16;
    Ok_Make_Hash(&ok_context, str, 7, hash, outlen);

    int i;
    for(i = 0;i < 16;i++)
    {
        printf("%x ", (unsigned char)hash[i]);
    }
    printf("\n");
    */


    /*
    char outp[4000];
    *((int *) outp) = out;
    memcpy(outp+4, hash, 16);
    char out2[4000];
    ioctl(ok_context.fs, OK_SIGN_HASH, outp);

    *((int *) out2) = out;
    memcpy(out2+4, hash, 16);

    *((int *) (out2+20)) = *((int *)outp);
    memcpy(out2+24, outp+4, *((int *)outp));
    ioctl(ok_context.fs, OK_VERIFY_HASH, out2);
    printf("verify: %d\n", *((int *)out2));
    */
    /*  
    char outp[4000];
    int outlenp = 1024;
    Ok_Sign_Hash(&ok_context, hash, 16, outp, &outlenp, out);

    int ret;
    Ok_Verify_Hash(&ok_context, hash, 16, outp, outlenp, &ret, out);

    printf("ver:%d\n", ret);
    */
    
    Ok_Free_Context(&ok_context);

    return 0;
}


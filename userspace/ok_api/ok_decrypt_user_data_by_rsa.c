/*
 * =====================================================================================
 *
 *       Filename:  ok_create_srk.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/12/2013 10:56:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ok_const.h"
#include "ok_api_const.h"
#include "ok_api_all.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"

OK_RESULT Ok_Decrypt_User_Data_By_Rsa(struct OK_CONTEXT * ok_context, unsigned char *in, unsigned int inlen,
        unsigned char * out, unsigned int *outlen, OK_KEY handle)
{
    if(ok_context == NULL)
        return OK_VALUE_ERROR;

    if(ok_context->fs < 0)
        return OK_FILE_OPEN_ERROR;

    int tmplen = inlen * 2;
    if(tmplen < 128*3)
        tmplen = 128*3;

    unsigned char *tmp = (unsigned char *) malloc(tmplen);
    *(int *)(tmp) = handle;
    *((int *)(tmp + 4)) = inlen;
    memcpy(tmp+8, in, inlen);
    ioctl(ok_context->fs, OK_RSA_DECRYPT, tmp);

    int outlen_tmp = *((int *)tmp);
    if(outlen_tmp > *outlen)
    {
        printf("outlen tmp size error!\n");

        return OK_VALUE_ERROR;
    }
    *outlen = outlen_tmp;
    memcpy(out, tmp+4, *outlen);

    free(tmp);
    tmp = NULL;

    return OK_SUCCESS;
}




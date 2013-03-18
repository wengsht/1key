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
#include "stdio.h"
#include "stdlib.h"

OK_RESULT Ok_Sign_Hash(struct OK_CONTEXT * ok_context, unsigned char *hash, unsigned int hashlen,
        unsigned char * out, unsigned int *outlen, OK_KEY handle)
{
    if(ok_context == NULL)
        return OK_VALUE_ERROR;

    if(ok_context->fs < 0)
        return OK_FILE_OPEN_ERROR;

    if(hashlen != OK_BIN_HASH_LEN)
        return OK_VALUE_ERROR;

    int tmplen = OK_MP_LEN *3;

    unsigned char *tmp = (unsigned char *) malloc(tmplen);
    *(int *)(tmp) = handle;
    memcpy(tmp+4, hash, hashlen);
    ioctl(ok_context->fs, OK_SIGN_HASH, tmp);

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

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

OK_RESULT Ok_Make_Hash(struct OK_CONTEXT * ok_context, unsigned char *in, int inlen, unsigned char *hash, int hash_len)
{
    if(ok_context == NULL)
        return OK_VALUE_ERROR;

    if(ok_context->fs < 0)
        return OK_FILE_OPEN_ERROR;

    if(hash_len < OK_BIN_HASH_LEN)
    {
        return OK_VALUE_ERROR;
    }

    int tmplen = inlen + 4;
    if(tmplen < OK_BIN_HASH_LEN)
        tmplen = OK_BIN_HASH_LEN;
    char *tmp = (char *) malloc(tmplen);
    *(int *)(tmp) = inlen;
    memcpy(tmp+4, in, inlen);
    ioctl(ok_context->fs, OK_MAKE_HASH, tmp);

    memcpy(hash, tmp, OK_BIN_HASH_LEN);

    free(tmp);
    tmp = NULL;

    return OK_SUCCESS;
}




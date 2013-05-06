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

OK_RESULT Ok_Load_User_Wrap_Rsa(struct OK_CONTEXT * ok_context,OK_KEY parent, char *filename, OK_KEY *handle)
{

    if(ok_context == NULL)
        return OK_VALUE_ERROR;

    if(ok_context->fs < 0)
        return OK_FILE_OPEN_ERROR;

    char *tmp = (char *) malloc(strlen(filename) + 9);
    *(int *)(tmp) = parent;
    *(int *)(tmp+4) = strlen(filename);
    memcpy(tmp+8, filename, strlen(filename));
    ioctl(ok_context->fs, OK_USER_WRAP_RSA_LOAD, tmp);

    *handle = *((int *)tmp);

    free(tmp);
    tmp = NULL;

    if((int)(*handle) == -1)
        return OK_VALUE_ERROR;
    else 
        return OK_SUCCESS;
}




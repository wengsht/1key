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
#include "ok_log.h"

OK_RESULT Ok_Create_User_Wrap_Aes(struct OK_CONTEXT * ok_context,OK_KEY parent, char *filename)
{
    if(ok_context == NULL)
        return OK_VALUE_ERROR;

    if(ok_context->fs < 0)
        return OK_FILE_OPEN_ERROR;

    int fs = open(filename, O_CREAT, S_IRUSR | S_IWUSR);
    if(fs <= 0)
        OKDEBUG("open file error");
    close(fs);

    char *tmp = (char *) malloc(strlen(filename) + 9);
    *(int *)(tmp) = parent;
    *(int *)(tmp+4) = strlen(filename);
    memcpy(tmp+8, filename, strlen(filename));
    ioctl(ok_context->fs, OK_USER_WRAP_AES_CREATE, tmp);

    free(tmp);
    tmp = NULL;

    return OK_SUCCESS;
}




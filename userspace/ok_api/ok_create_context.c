/*
 * =====================================================================================
 *
 *       Filename:  ok_create_context.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 03:08:09 PM
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

OK_RESULT Ok_Create_Context(struct OK_CONTEXT * ok_context)
{

    if(ok_context == NULL)
        return OK_VALUE_ERROR;
    ok_context->fs = open(DEV_FILE_NAME, O_RDWR);

    if(ok_context->fs < 0)
        return OK_FILE_OPEN_ERROR;

    return OK_SUCCESS;
}




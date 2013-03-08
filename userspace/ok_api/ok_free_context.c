/*
 * =====================================================================================
 *
 *       Filename:  ok_free_context.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 03:21:03 PM
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

OK_RESULT Ok_Free_Context(struct OK_CONTEXT *ok_context)
{
    if(ok_context == NULL)
        return OK_VALUE_ERROR;

    if(ok_context->fs < 0)
        return OK_VALUE_ERROR;

    close(ok_context->fs);

    return OK_SUCCESS;
}

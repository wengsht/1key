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

#include "ok_const.h"
#include "ok_api_const.h"
#include "ok_api_all.h"

int main()
{
    struct OK_CONTEXT ok_context;

    Ok_Create_Context(&ok_context);

    Ok_Print_Version(&ok_context);

    //ioctl(ok_context.fs, OK_SRK_CREATE);
    ioctl(ok_context.fs, OK_SRK_LOAD);

    Ok_Free_Context(&ok_context);

    return 0;
}


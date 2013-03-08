/*
 * =====================================================================================
 *
 *       Filename:  ok_print_version.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 02:44:12 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ok_const.h"
#include "ok_struct.h"
#include "ok_api_const.h"
#include "ok_api_all.h"


OK_RESULT Ok_Print_Version(struct OK_CONTEXT *ok_context)
{
    struct Ok_version tmp;
    ioctl(ok_context->fs, OK_VERSION, &tmp);
    printf("Device Name: %s\n", tmp.ok_name);
    printf("Version:     %.2lf\n", (double)tmp.ok_version_id * 0.01);
    printf("Author:      %s\n", tmp.ok_author_name);
}



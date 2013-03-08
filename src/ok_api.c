/*
 * =====================================================================================
 *
 *       Filename:  ok_api.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 10:35:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ok.h"


OK_RESULT ok_print_version(unsigned long arg)
{
    OK_RESULT res;
    struct Ok_version * ok_version;
    ok_version = (struct Ok_version *)kmalloc(sizeof(struct Ok_version), GFP_KERNEL);

    strcpy(ok_version->ok_name, OK_DEFAULT_NAME);
    ok_version->ok_version_id =  OK_VERSION_ID;
    strcpy(ok_version->ok_author_name, OK_AUTHOR_NAME);

    res = copy_to_user((void *)arg, ok_version, sizeof(struct Ok_version));

    kfree(ok_version);
    ok_version = NULL;

    return res;
}

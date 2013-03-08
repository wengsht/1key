/*
 * =====================================================================================
 *
 *       Filename:  ok_struct.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 10:44:53 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef _OK_STRUCT_H_
#define _OK_STRUCT_H_

#include "ok.h"

struct Ok_version
{
    char ok_name[OK_MAX_VERSION_NAME_LEN];
    unsigned long ok_version_id;
};

#endif

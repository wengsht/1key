/*
 * =====================================================================================
 *
 *       Filename:  ok_const.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 02:17:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_CONST_H_
#define __OK_CONST_H_

typedef unsigned int OK_RESULT;

// version 
#define OK_MAX_VERSION_NAME_LEN 25
#define OK_DEFAULT_NAME "One Key Crypto"
#define OK_VERSION_ID 1

// Error Code
#define OK_SUCCESS 0

// ioctl
#define OK_MAGIC 'o'
#define OK_VERSION _IOR(OK_MAGIC, 1, struct Ok_version)

#endif

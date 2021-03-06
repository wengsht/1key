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
typedef unsigned int OK_KEY;

// version 
#define OK_MAX_VERSION_NAME_LEN 25
#define OK_MAX_AUTHOR_NAME_LEN 15
#define OK_DEFAULT_NAME "One Key Crypto"
#define OK_AUTHOR_NAME "wengsht@SYSU"
#define OK_VERSION_ID 1

// Error Code
#define OK_SUCCESS 0
#define OK_FILE_OPEN_ERROR -11
#define OK_VALUE_ERROR -12
#define OK_KEY_CREATE_ERROR -13
#define OK_SRK_NOT_FOUND -14

#define OK_RSA_KEY_SIZE 128
#define E_VALUE  65537

#define OK_BIN_HASH_LEN 16
#define OK_MP_LEN 128
// ioctl version
#define OK_MAGIC 'o'
#define OK_VERSION _IOR(OK_MAGIC, 1, struct Ok_version)

// SRK
#define OK_SRK_CREATE _IO(OK_MAGIC, 2)
#define OK_SRK_LOAD _IO(OK_MAGIC, 3)
#define OK_CLEAR _IO(OK_MAGIC, 4)

// RSA
#define OK_USER_RSA_CREATE _IOW(OK_MAGIC, 5, int)
#define OK_USER_RSA_LOAD   _IOWR(OK_MAGIC, 6, int)
#define OK_RSA_ENCRYPT _IOWR(OK_MAGIC, 7, int)
#define OK_RSA_DECRYPT _IOWR(OK_MAGIC, 8, int)

// HASH 
#define OK_MAKE_HASH _IOWR(OK_MAGIC, 9, int)
#define OK_SIGN_HASH _IOWR(OK_MAGIC, 10, int)
#define OK_VERIFY_HASH _IOWR(OK_MAGIC, 11, int)

// AES 
#define OK_USER_AES_CREATE _IOW(OK_MAGIC, 12, int)
#define OK_USER_AES_LOAD   _IOWR(OK_MAGIC, 13, int)
#define OK_AES_ENCRYPT     _IOWR(OK_MAGIC, 14, int)
#define OK_AES_DECRYPT     _IOWR(OK_MAGIC, 15, int)

// WRAP KEYS
#define OK_USER_WRAP_RSA_CREATE  _IOW(OK_MAGIC, 16, int)
#define OK_USER_WRAP_RSA_LOAD    _IOWR(OK_MAGIC, 17, int)
#define OK_USER_WRAP_AES_CREATE  _IOW(OK_MAGIC, 18, int)
#define OK_USER_WRAP_AES_LOAD    _IOWR(OK_MAGIC, 19, int)

#define AES_KEY_LEN 16
#define AES_BLOCK_LEN 16
#define OK_MAX_PASSWD_LEN 56

#endif

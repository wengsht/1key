/*
 * =====================================================================================
 *
 *       Filename:  ok_api_all.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 03:10:43 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_API_ALL_H_
#define __OK_API_ALL_H_

#include "ok_struct.h"
#include "ok_const.h"
#include "ok_api_const.h"
#include <stddef.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

OK_RESULT Ok_Create_Context(struct OK_CONTEXT * );

OK_RESULT Ok_Free_Context(struct OK_CONTEXT *);

OK_RESULT Ok_Print_Version(struct OK_CONTEXT *);

OK_RESULT Ok_Create_Srk(struct OK_CONTEXT * ok_context);

OK_RESULT Ok_Load_Srk(struct OK_CONTEXT * ok_context);


OK_RESULT Ok_Create_User_Rsa(struct OK_CONTEXT * ok_context, char *filename);

OK_RESULT Ok_Load_User_Rsa(struct OK_CONTEXT * ok_context, char *filename, OK_KEY *handle);

OK_RESULT Ok_Encrypt_User_Data_By_Rsa(struct OK_CONTEXT * ok_context, unsigned char *in, unsigned int inlen,
        unsigned char * out, unsigned int *outlen, OK_KEY handle);

OK_RESULT Ok_Decrypt_User_Data_By_Rsa(struct OK_CONTEXT * ok_context, unsigned char *in, unsigned int inlen,
        unsigned char * out, unsigned int *outlen, OK_KEY handle);

OK_RESULT Ok_Sign_Hash(struct OK_CONTEXT * ok_context, unsigned char *hash, unsigned int hashlen,
        unsigned char * out, unsigned int *outlen, OK_KEY handle);

OK_RESULT Ok_Verify_Hash(struct OK_CONTEXT * ok_context, unsigned char *hash, unsigned int hashlen,
        unsigned char * blob, unsigned int bloblen, int *ret, OK_KEY handle);
#endif

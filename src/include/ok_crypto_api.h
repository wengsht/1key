/*
 * =====================================================================================
 *
 *       Filename:  ok_crypto_api.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2013 05:10:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_CRYPTO_API_H_
#define __OK_CRYPTO_API_H_

#include "tomcrypt.h"
OK_RESULT rsa_encrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key);

OK_RESULT rsa_decrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key);

OK_RESULT ok_encrypt_rsa_by_srk(rsa_key *in, unsigned char *blob);
OK_RESULT ok_decrypt_rsa_by_srk(rsa_key *in, unsigned char *blob);



#endif

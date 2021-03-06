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

OK_RESULT rsa_encrypt_data_extend(const unsigned char *in, unsigned inlen,
        unsigned char *blob, unsigned long *outlen, rsa_key *key);

OK_RESULT rsa_decrypt_data_extend(const unsigned char *in, unsigned inlen,
        unsigned char *blob, unsigned long *outlen, rsa_key *key);

OK_RESULT ok_make_hash(unsigned char *in, int inlen, unsigned char *hash, int *outlen);

OK_RESULT ok_encrypt_aes_by_srk(unsigned char * aes_key, unsigned char *blob);
OK_RESULT ok_decrypt_aes_by_srk(unsigned char *aes_key, unsigned char *blob);

OK_RESULT aes_encrypt_data_extend(unsigned char *in,int inlen, unsigned char *out, int *outlen,unsigned char * key);
OK_RESULT aes_decrypt_data_extend(unsigned char *in,int inlen, unsigned char *out, int *outlen,unsigned char * key);
#endif

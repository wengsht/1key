/*
 * =====================================================================================
 *
 *       Filename:  ok_crypto_api.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/09/2013 05:05:29 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "ok.h"
#include "ok_const.h"
#include "tomcrypt.h"

OK_RESULT rsa_encrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key)
{
    return rsa_encrypt_key_ex(in, inlen, out, outlen, NULL, 0, NULL, LTC_LTC_PKCS_1_V1_5, key);
}


OK_RESULT rsa_decrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key)
{
    int valid;
    return rsa_decrypt_key_ex(in, inlen, out, outlen, NULL, 0, NULL, LTC_LTC_PKCS_1_V1_5, &valid, key);
}



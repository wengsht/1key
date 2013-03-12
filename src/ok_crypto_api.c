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
#include "tommath.h"

OK_RESULT rsa_encrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key)
{
    if(*outlen <= 0)
        *outlen = mp_unsigned_bin_size(&(key->N));
    return rsa_encrypt_key_ex(in, inlen, out, outlen, NULL, 0, NULL, LTC_LTC_PKCS_1_V1_5, key);
}


OK_RESULT rsa_decrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key)
{
    if(*outlen <= 0)
        *outlen = mp_unsigned_bin_size(&(key->N));
    int valid;
    return rsa_decrypt_key_ex(in, inlen, out, outlen, NULL, 0, NULL, LTC_LTC_PKCS_1_V1_5, &valid, key);
}

static OK_RESULT ok_encrypt_mp_by_rsa(mp_int *in, rsa_key *key, unsigned char *blob, int *pos)
{
    int outlen = mp_unsigned_bin_size(&(key->N)), outlen2 = outlen;
    unsigned char * out, *out2;
    out = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(key->N)), GFP_KERNEL);
    out2 = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(key->N)), GFP_KERNEL);
    mp_to_unsigned_bin_n(in, out, &outlen);

//    mp_read_unsigned_bin(in, out, outlen);

    int outlen3 = outlen / 2;
    int outlen4 = outlen - outlen3;
    rsa_encrypt_data(out, outlen3, out2, &outlen2, key);

    *((int *)(blob+(*pos))) = outlen2;
    memcpy(blob + (*pos) + 4, out2, outlen2);

    (*pos) += (outlen2 + 4);

    outlen2 = mp_unsigned_bin_size(&(key->N));
    rsa_encrypt_data(out+outlen3, outlen4, out2, &outlen2, key);
    *((int *)(blob+(*pos))) = outlen2;
    memcpy(blob + (*pos) + 4, out2, outlen2);
    (*pos) += (outlen2 + 4);

    kfree(out);
    kfree(out2);

    return OK_SUCCESS;
}
static OK_RESULT ok_decrypt_mp_by_rsa(mp_int *in, rsa_key *key, unsigned char *blob, int *pos)
{
    int outlen = mp_unsigned_bin_size(&(key->N)), outlen2 = outlen, outlen3 = outlen2;
    unsigned char * out, *out2;
    out = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(key->N)), GFP_KERNEL);
    out2 = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(key->N)), GFP_KERNEL);

    outlen = *((int *)(blob + (*pos)));
    memcpy(out, blob+(*pos)+4, outlen);

    rsa_decrypt_data(out, outlen, out2, &outlen2, key);

    (*pos) += (outlen + 4);

    outlen = *((int *)(blob + (*pos)));
    memcpy(out, blob+(*pos)+4, outlen);


    rsa_decrypt_data(out, outlen, out2 + outlen2, &outlen3, key);

    mp_read_unsigned_bin(in, out2, outlen2 + outlen3);

    (*pos) += (4 + outlen);

    kfree(out);
    kfree(out2);
    return OK_SUCCESS;
}

OK_RESULT ok_encrypt_rsa_by_srk(rsa_key *in, unsigned char *blob)
{
    if(srk_key == NULL)
    {
        OKDEBUG("SRK KEY NOT LOADED!\n");

        return OK_SRK_NOT_FOUND;
    }
    int pos = 4;
    *(int *)(blob + pos) = in->type;
    pos += 4;

    ok_encrypt_mp_by_rsa(&(in->e), srk_key, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->d), srk_key, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->N), srk_key, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->p), srk_key, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->q), srk_key, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->qP), srk_key,blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->dP), srk_key,blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->dQ), srk_key,blob, &pos);

    *((int *)blob) = pos - 4;

    return OK_SUCCESS;
}
OK_RESULT ok_decrypt_rsa_by_srk(rsa_key *in, unsigned char *blob)
{
    if(srk_key == NULL)
    {
        OKDEBUG("SRK KEY NOT LOADED!\n");

        return OK_SRK_NOT_FOUND;
    }
    int pos = 4;
    in->type = *((int *)(blob + pos));
    pos += 4;
    ok_decrypt_mp_by_rsa(&(in->e), srk_key, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->d), srk_key, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->N), srk_key, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->p), srk_key, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->q), srk_key, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->qP), srk_key,blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->dP), srk_key,blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->dQ), srk_key,blob, &pos);

    return OK_SUCCESS;
}

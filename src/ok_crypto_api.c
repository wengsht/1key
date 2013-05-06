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
        *outlen = OK_MP_LEN;
    return rsa_encrypt_key_ex(in, inlen, out, outlen, NULL, 0, NULL, LTC_LTC_PKCS_1_V1_5, key);
}


OK_RESULT rsa_decrypt_data(const unsigned char *in, unsigned inlen, 
        unsigned char * out, unsigned long *outlen,
        rsa_key *key)
{
    if(*outlen <= 0)
        *outlen = OK_MP_LEN;
    int valid;
    return rsa_decrypt_key_ex(in, inlen, out, outlen, NULL, 0, NULL, LTC_LTC_PKCS_1_V1_5, &valid, key);
}

OK_RESULT rsa_encrypt_data_extend(const unsigned char *in, unsigned inlen,
        unsigned char *blob, unsigned long *outlen, rsa_key *key)
{
    int mx_out = *outlen;

    int block_size = OK_MP_LEN - 28;
    int block_out_len, block_in_len ;
    int cnt = 0;

    int pos = 0;

    *outlen = 4;  
    while(pos < inlen)
    {
        block_out_len = OK_MP_LEN;
        block_in_len = min(block_size, inlen - pos);
        
        if(*outlen + 4 + block_out_len > mx_out)
        {
            OKDEBUG("mx len\n");

            return OK_VALUE_ERROR;
        }
        rsa_encrypt_data(in + pos, block_in_len, blob + (*outlen) + 4, &block_out_len, key);

        *((int *)(blob + (*outlen))) = block_out_len;
        *outlen += 4 + block_out_len;
        cnt ++;
        pos += block_in_len;
    }

    *((int *) blob) = cnt;

    return OK_SUCCESS;
}
OK_RESULT rsa_decrypt_data_extend(const unsigned char *in, unsigned inlen,
        unsigned char *blob, unsigned long *outlen, rsa_key *key)
{
    int mx_len = *outlen;

    *outlen = 0;
    int cnt = *((int *) in);

    int block_out_len, block_in_len;
    int pos = 4;
    while(cnt --)
    {
        block_out_len = mp_unsigned_bin_size(&(key->N));
        block_in_len = *((int *)(in + pos));
        if(pos + block_in_len + 4 > inlen)
        {
            OKDEBUG("mx in len\n");

            return OK_VALUE_ERROR;
        }

        if(*outlen + block_out_len > mx_len)
        {
            OKDEBUG("de mx len\n");

            return OK_VALUE_ERROR;
        }

        rsa_decrypt_data(in + pos + 4, block_in_len, blob + (*outlen), &block_out_len, key);
        *outlen += block_out_len;
        pos += block_in_len + 4;
    }
    return OK_SUCCESS;
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

OK_RESULT ok_encrypt_rsa_by_rsa(rsa_key *in, rsa_key *fa, unsigned char * blob)
{
    if(fa == NULL)
    {
        OKDEBUG("RSA KEY NOT LOADED!\n");

        return OK_SRK_NOT_FOUND;
    }
    int pos = 4;
    *(int *)(blob + pos) = in->type;
    pos += 4;

    ok_encrypt_mp_by_rsa(&(in->e), fa, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->d), fa, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->N), fa, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->p), fa, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->q), fa, blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->qP), fa,blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->dP), fa,blob, &pos);
    ok_encrypt_mp_by_rsa(&(in->dQ), fa,blob, &pos);

    *((int *)blob) = pos - 4;

    return OK_SUCCESS;
}
OK_RESULT ok_encrypt_rsa_by_srk(rsa_key *in, unsigned char *blob)
{
    return ok_encrypt_rsa_by_rsa(in, srk_key, blob);
}
OK_RESULT ok_decrypt_rsa_by_rsa(rsa_key *in,rsa_key *fa, unsigned char *blob)
{
    if(fa == NULL)
    {
        OKDEBUG("SRK KEY NOT LOADED!\n");

        return OK_SRK_NOT_FOUND;
    }
    int pos = 4;
    in->type = *((int *)(blob + pos));
    pos += 4;
    ok_decrypt_mp_by_rsa(&(in->e), fa, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->d), fa, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->N), fa, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->p), fa, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->q), fa, blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->qP), fa,blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->dP), fa,blob, &pos);
    ok_decrypt_mp_by_rsa(&(in->dQ), fa,blob, &pos);

    return OK_SUCCESS;
}

OK_RESULT ok_decrypt_rsa_by_srk(rsa_key *in, unsigned char *blob)
{
    return ok_decrypt_rsa_by_rsa(in, srk_key, blob);
}

OK_RESULT ok_make_hash(unsigned char *in, int inlen, unsigned char *hash, int *outlen)
{
    if(*outlen < OK_BIN_HASH_LEN)
    {
        OKDEBUG("HASH LEN LIMIT\n");
        return OK_VALUE_ERROR;
    }
    struct shash_desc * sdescmd5;

    struct crypto_shash * md5 = crypto_alloc_shash("md5", 0, 0);
    if(IS_ERR(md5))
        return ;

    int size = sizeof(struct shash_desc) + crypto_shash_descsize(md5);
    sdescmd5 = kmalloc(size, GFP_KERNEL);

    sdescmd5->tfm = md5;
    sdescmd5->flags = 0x0;
    crypto_shash_init(sdescmd5);

    crypto_shash_update(sdescmd5, in, inlen);
    crypto_shash_final(sdescmd5, hash);

    kfree(sdescmd5);
    crypto_free_shash(md5);
}
OK_RESULT ok_encrypt_aes_by_rsa(unsigned char * aes_key,rsa_key *fa, unsigned char *blob)
{
    int inlen = AES_KEY_LEN;
    *((int *)blob) = OK_MP_LEN * 3;

    rsa_encrypt_data_extend(aes_key, inlen, blob+4, (int *)blob, fa);

    return OK_SUCCESS;

}
OK_RESULT ok_encrypt_aes_by_srk(unsigned char * aes_key, unsigned char *blob)
{
    return ok_encrypt_aes_by_rsa(aes_key, srk_key, blob);
}
OK_RESULT ok_decrypt_aes_by_rsa(unsigned char *aes_key, rsa_key *fa, unsigned char *blob)
{
    int outlen = OK_MP_LEN * 2;
    rsa_decrypt_data_extend(blob+4,*((int *)blob), aes_key, &outlen, fa);

    if(outlen != AES_KEY_LEN)
        return OK_VALUE_ERROR;
    return OK_SUCCESS;
}
OK_RESULT ok_decrypt_aes_by_srk(unsigned char *aes_key, unsigned char *blob)
{
    return ok_decrypt_aes_by_rsa(aes_key, srk_key, blob);

}

OK_RESULT aes_encrypt_data_extend(unsigned char *in,int inlen, unsigned char *out, int *outlen,unsigned char * key)
{
	struct crypto_cipher *tfm;

	tfm = crypto_alloc_cipher("aes", 0, CRYPTO_ALG_ASYNC);
	if (!IS_ERR(tfm))
		crypto_cipher_setkey(tfm, key, 16);

    int tmp_in_len = 0;
    int tmp_out_len = 0;

    while(tmp_in_len < inlen)
    {
        if(tmp_out_len + AES_BLOCK_LEN > *outlen)
            return OK_VALUE_ERROR;

        crypto_cipher_encrypt_one(tfm, out+tmp_out_len, in + tmp_in_len);
        tmp_in_len += AES_BLOCK_LEN;
        tmp_out_len += AES_BLOCK_LEN;
    }

	crypto_free_cipher(tfm);

    return OK_SUCCESS;
}
OK_RESULT aes_decrypt_data_extend(unsigned char *in,int inlen, unsigned char *out, int *outlen,unsigned char * key)
{
	struct crypto_cipher *tfm;

	tfm = crypto_alloc_cipher("aes", 0, CRYPTO_ALG_ASYNC);
	if (!IS_ERR(tfm))
		crypto_cipher_setkey(tfm, key, 16);

    int tmp_in_len = 0;
    int tmp_out_len = 0;

    while(tmp_in_len < inlen)
    {
        if(tmp_out_len + AES_BLOCK_LEN > *outlen)
            return OK_VALUE_ERROR;

        crypto_cipher_decrypt_one(tfm, out+tmp_out_len, in + tmp_in_len);
        tmp_in_len += AES_BLOCK_LEN;
        tmp_out_len += AES_BLOCK_LEN;
    }

	crypto_free_cipher(tfm);

    return OK_SUCCESS;
}

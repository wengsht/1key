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
#include "ok_test.h"
#include "tomcrypt.h"

#include "ok_crypto_api.h"
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
static OK_RESULT vfs_store_mp_int(struct file *fp, unsigned char *buf, mp_int in, loff_t *pos)
{
    (*(int *)buf) = (in.used) * sizeof(mp_digit);
    memcpy(buf + sizeof(int), &in, sizeof(int) * 3);
    memcpy(buf + sizeof(int) * 4, in.dp, (*(int *)buf));

    vfs_write(fp, buf, sizeof(int) * 4 + *((int *)buf), pos);

    return OK_SUCCESS;
}
static OK_RESULT vfs_load_mp_int(struct file *fp, unsigned char *buf, mp_int *in, loff_t *pos)
{
    vfs_read(fp, buf, sizeof(int), pos);
    
    vfs_read(fp, in, sizeof(int) * 3, pos); 

    vfs_read(fp, in->dp, *((int *)buf), pos); 
}

OK_RESULT ok_store_rsa_key(rsa_key *key, char *file_name)
{
    unsigned char *tmp = (unsigned char *) kmalloc(sizeof(mp_digit) * (key->N).used + sizeof(int) * 4, GFP_KERNEL);

    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    fp = filp_open(file_name, O_RDWR | O_CREAT, 0644);

    if(IS_ERR(fp))
    {
        OKDEBUG("open file error\n");

        return OK_FILE_OPEN_ERROR;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);

    pos = 0;

    /*  type  */
    *((int *)tmp) = key->type;
    vfs_write(fp, tmp, sizeof(int), &pos);

    /*  e */
    vfs_store_mp_int(fp, tmp, key->e, &pos);

    /*  d */
    vfs_store_mp_int(fp, tmp, key->d, &pos);

    /*  N */
    vfs_store_mp_int(fp, tmp, key->N, &pos);

    /*  p */
    vfs_store_mp_int(fp, tmp, key->p, &pos);

    /*  q */
    vfs_store_mp_int(fp, tmp, key->q, &pos);

    /*  qP */
    vfs_store_mp_int(fp, tmp, key->qP, &pos);

    /*  dP */
    vfs_store_mp_int(fp, tmp, key->dP, &pos);

    /*  dQ */
    vfs_store_mp_int(fp, tmp, key->dQ, &pos);

    filp_close(fp, NULL);
    set_fs(fs);

    kfree(tmp);
    tmp = NULL;
    return OK_SUCCESS;
}
OK_RESULT ok_store_srk(rsa_key *key)
{
    return ok_store_rsa_key(key, SRK_KEY_FILE);
}

OK_RESULT ok_create_key(rsa_key **key)
{
    *key = (rsa_key *) kmalloc(sizeof(rsa_key), GFP_KERNEL);
    
    if(OK_SUCCESS != rsa_make_key(OK_RSA_KEY_SIZE, E_VALUE, *key))
        return OK_KEY_CREATE_ERROR;

    return OK_SUCCESS;
}

OK_RESULT ok_create_srk(rsa_key **key)
{
    ok_create_key(key);

    ok_store_srk(*key);

    ok_free_rsa_key(key);

    return OK_SUCCESS;
}
static OK_RESULT ok_store_blob(unsigned char *blob, char *filename)
{
    struct file *fp;
    mm_segment_t fs;
    loff_t pos = 0;
    fp = filp_open(filename, O_RDWR | O_CREAT, 0644);

    if(IS_ERR(fp))
    {
        OKDEBUG("open file error\n");

        return OK_FILE_OPEN_ERROR;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);

    vfs_write(fp, blob, *((int *)blob) + 4, &pos);

    filp_close(fp, NULL);
    set_fs(fs);
}
static OK_RESULT ok_load_blob(unsigned char *blob, char *filename)
{
    struct file *fp;
    mm_segment_t fs;
    loff_t pos = 0;
    fp = filp_open(filename, O_RDWR | O_CREAT, 0644);

    if(IS_ERR(fp))
    {
        OKDEBUG("open file error\n");

        return OK_FILE_OPEN_ERROR;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);


    vfs_read(fp, blob, 4, &pos);
    vfs_read(fp, blob+4, *((int *)blob), &pos); 


    filp_close(fp, NULL);
    set_fs(fs);
}
OK_RESULT ok_create_user_rsa(unsigned long __user arg)
{
    int len;
    copy_from_user(&len, (void *)arg, 4);
    char *tmp= (char *)kmalloc(len+5, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+4);
    tmp[len+4] = '\0';

    char *filename = tmp + 4;


    rsa_key * user_key;
    ok_create_key(&user_key);

    unsigned char *blob = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(user_key->N)) * 20, GFP_KERNEL);

    ok_encrypt_rsa_by_srk(user_key, blob);

    ok_store_blob(blob, filename);

    ok_free_rsa_key(&user_key);

    kfree(tmp);
    kfree(blob);
    tmp = NULL;

    return OK_SUCCESS;
}
OK_RESULT ok_load_user_rsa(unsigned long __user arg)
{
    int len;
    copy_from_user(&len, (void *)arg, 4);
    char *tmp= (char *)kmalloc(len+5, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+4);
    tmp[len+4] = '\0';

    char *filename = tmp + 4;

    rsa_key * user_key;


    ok_create_key(&user_key);

    unsigned char *blob = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(user_key->N)) * 20, GFP_KERNEL);
    ok_load_blob(blob, filename);

    ok_decrypt_rsa_by_srk(user_key, blob);

    copy_to_user((void *)arg, &user_key, sizeof(rsa_key *));

    kfree(blob);
    kfree(tmp);

    return OK_SUCCESS;
}
OK_RESULT ok_free_rsa_key(rsa_key **key)
{
    OKDEBUG("free rsa key ! \n ");
    if(*key != NULL)
    {
        rsa_free(*key);

        kfree(*key);
        *key = NULL;
    }

    return OK_SUCCESS;
}
OK_RESULT ok_load_rsa_key(rsa_key **key, char *file_name)
{
    ok_free_rsa_key(key);
    ok_create_key(key);

    unsigned char *tmp = (unsigned char *) kmalloc(sizeof(mp_digit) * MP_PREC + sizeof(int) * 3, GFP_KERNEL);

    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    fp = filp_open(file_name, O_RDWR, 0644);

    if(IS_ERR(fp))
    {
        OKDEBUG("open file error\n");

        return OK_FILE_OPEN_ERROR;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);

    pos = 0;

    /*  type  */
    vfs_read(fp, tmp, sizeof(int), &pos);
    (*key)->type = *((int *)tmp);

    /*  e */
    vfs_load_mp_int(fp, tmp, &((*key)->e), &pos);

    /*  d */
    vfs_load_mp_int(fp, tmp, &((*key)->d), &pos);

    /*  N */
    vfs_load_mp_int(fp, tmp, &((*key)->N), &pos);

    /*  p */
    vfs_load_mp_int(fp, tmp, &((*key)->p), &pos);

    /*  q */
    vfs_load_mp_int(fp, tmp, &((*key)->q), &pos);

    /*  qP */
    vfs_load_mp_int(fp, tmp, &((*key)->qP), &pos);

    /*  dP */
    vfs_load_mp_int(fp, tmp, &((*key)->dP), &pos);

    /*  dQ */
    vfs_load_mp_int(fp, tmp, &((*key)->dQ), &pos);

    filp_close(fp, NULL);
    set_fs(fs);

    kfree(tmp);
    tmp = NULL;
    return OK_SUCCESS;
}
OK_RESULT ok_load_srk(rsa_key **key)
{
    return ok_load_rsa_key(key, SRK_KEY_FILE);
}

OK_RESULT ok_rsa_encrypt_user_data(unsigned long arg)
{
    rsa_key *key;
    int inlen, outlen;
    unsigned char *in, *out;
    copy_from_user((void *)(&key), (void *)arg, sizeof(rsa_key *));
    copy_from_user(&inlen, (unsigned char *)arg + sizeof(rsa_key *), sizeof(int));
    in = (unsigned char *)kmalloc(inlen, GFP_KERNEL);
    outlen = inlen * 2;
    outlen = max(outlen, mp_unsigned_bin_size(&(key->N)) * 3);
    out = (unsigned char *)kmalloc(outlen, GFP_KERNEL);

    copy_from_user(in, (unsigned char *)arg + sizeof(rsa_key *) + sizeof(int), inlen);

    rsa_encrypt_data_extend(in, inlen, out+4, &outlen, key);
    *(int *)out = outlen;

    copy_to_user((void *)arg, out, outlen + 4);
    kfree(out);
    kfree(in);

    return OK_SUCCESS;
}
OK_RESULT ok_rsa_decrypt_user_data(unsigned long arg)
{
    rsa_key *key;
    int inlen, outlen;
    unsigned char *in, *out;
    copy_from_user((void *)(&key), (void *)arg, sizeof(rsa_key *));
    copy_from_user(&inlen, (unsigned char *)arg + sizeof(rsa_key *), sizeof(int));
    in = (unsigned char *)kmalloc(inlen, GFP_KERNEL);
    outlen = inlen * 2;
    outlen = max(outlen, mp_unsigned_bin_size(&(key->N)) * 3);
    out = (unsigned char *)kmalloc(outlen, GFP_KERNEL);

    copy_from_user(in, (unsigned char *)arg + sizeof(rsa_key *) + sizeof(int), inlen);

    rsa_decrypt_data_extend(in, inlen, out+4, &outlen, key);
    *(int *)out = outlen;

    copy_to_user((void *)arg, out, outlen + 4);

    kfree(out);
    kfree(in);

    return OK_SUCCESS;
}

OK_RESULT ok_make_user_hash(unsigned long arg)
{ 
    int inlen;
    unsigned char * in;
    copy_from_user(&inlen, (void *)arg, 4);

    in = (unsigned char *)kmalloc(inlen, GFP_KERNEL);
    copy_from_user(in, (unsigned char *)(arg + 4), inlen);

    unsigned char hash[OK_BIN_HASH_LEN];
    int outlen = OK_BIN_HASH_LEN;

    ok_make_hash(in, inlen, hash, &outlen);

    copy_to_user((void *)arg, hash, OK_BIN_HASH_LEN);

    kfree(in);
    in = NULL;
    return OK_SUCCESS;
}

OK_RESULT ok_sign_user_hash(unsigned long arg)
{
    rsa_key *key;
    copy_from_user((void *)(&key), (void *)arg, sizeof(rsa_key *));
    unsigned char * hash = kmalloc(OK_BIN_HASH_LEN, GFP_KERNEL);
    unsigned char * out = kmalloc(OK_MP_LEN * 2, GFP_KERNEL);
    int outlen = OK_MP_LEN * 2;
    copy_from_user(hash, (unsigned char*)arg + 4, OK_BIN_HASH_LEN);

    rsa_encrypt_data(hash, OK_BIN_HASH_LEN, out, &outlen, key);

    copy_to_user((void *)arg, &outlen, 4);
    copy_to_user((unsigned char *)arg + 4, out, outlen);

    kfree(hash);
    kfree(out);

    return OK_SUCCESS;
}

OK_RESULT ok_verify_user_hash(unsigned long arg)
{
    rsa_key *key;
    copy_from_user((void *)(&key), (void *)arg, sizeof(rsa_key *));

    unsigned char * hash = kmalloc(OK_BIN_HASH_LEN, GFP_KERNEL);
    unsigned char * out = kmalloc(OK_MP_LEN * 2, GFP_KERNEL);
    unsigned char * tmp = kmalloc(OK_MP_LEN * 2, GFP_KERNEL);

    int tmplen, outlen = OK_MP_LEN * 2;
    copy_from_user(hash, (unsigned char*)arg + 4, OK_BIN_HASH_LEN);
    copy_from_user(&tmplen, (unsigned char *)arg + 20, 4);
    copy_from_user(tmp, (unsigned char *)arg + 24, tmplen);

    rsa_decrypt_data(tmp, tmplen, out, &outlen, key);

    int ret = 1;
    if(outlen != OK_BIN_HASH_LEN)
        ret = 0;
    else 
    {
        int i;
        for(i = 0;i < OK_BIN_HASH_LEN;i++)
        {
            if(hash[i] != out[i])
            {
                ret = 0;

                break;
            }
        }
    }
    copy_to_user((void *)arg, &ret, 4);

    kfree(hash);
    kfree(tmp);
    kfree(out);

    return OK_SUCCESS;
}
OK_RESULT ok_create_user_aes(unsigned long __user arg)
{
    int len;
    copy_from_user(&len, (void *)arg, 4);
    char *tmp= (char *)kmalloc(len+5, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+4);
    tmp[len+4] = '\0';

    char *filename = tmp + 4;


    unsigned char key[AES_KEY_LEN];
    get_random_bytes(key, AES_KEY_LEN);

    unsigned char *blob = (unsigned char *)kmalloc(OK_MP_LEN * 3, GFP_KERNEL);

    ok_encrypt_aes_by_srk(key, blob);

    ok_store_blob(blob, filename);

    kfree(tmp);
    kfree(blob);
    tmp = NULL;

    return OK_SUCCESS;
}
OK_RESULT ok_load_user_aes(unsigned long __user arg)
{
    int len;
    copy_from_user(&len, (void *)arg, 4);
    char *tmp= (char *)kmalloc(len+5, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+4);
    tmp[len+4] = '\0';

    char *filename = tmp + 4;

    unsigned char *aes_key = (unsigned char *)kmalloc(AES_KEY_LEN, GFP_KERNEL);

    unsigned char *blob = (unsigned char *)kmalloc(OK_MP_LEN * 3, GFP_KERNEL);
    ok_load_blob(blob, filename);

    ok_decrypt_aes_by_srk(aes_key, blob);

    copy_to_user((void *)arg, &aes_key, sizeof(unsigned char *));

    kfree(blob);
    kfree(tmp);

    return OK_SUCCESS;
}

OK_RESULT ok_aes_encrypt_user_data(unsigned long arg)
{
    unsigned char *key;
    int inlen, outlen;
    unsigned char *in, *out;
    copy_from_user((void *)(&key), (void *)arg, sizeof(unsigned char *));
    copy_from_user(&inlen, (unsigned char *)arg + sizeof(unsigned char *), sizeof(int));
    in = (unsigned char *)kmalloc(inlen, GFP_KERNEL);
    outlen = inlen * 2;
    outlen = max(outlen, OK_MP_LEN * 3);
    out = (unsigned char *)kmalloc(outlen, GFP_KERNEL);

    copy_from_user(in, (unsigned char *)arg + sizeof(unsigned char *) + sizeof(int), inlen);

    outlen -= 8;
    aes_encrypt_data_extend(in, inlen, out+8, &outlen, key);
    *(int *)out = outlen+4;
    *(int *)(out+4) = inlen;

    copy_to_user((void *)arg, out, outlen + 4);
    kfree(out);
    kfree(in);

    return OK_SUCCESS;
}
OK_RESULT ok_aes_decrypt_user_data(unsigned long arg)
{
    unsigned char *key;
    int inlen, outlen;
    unsigned char *in, *out;
    copy_from_user((void *)(&key), (void *)arg, sizeof(unsigned char *));
    copy_from_user(&inlen, (unsigned char *)arg + sizeof(unsigned char *), sizeof(int));
    in = (unsigned char *)kmalloc(inlen, GFP_KERNEL);
    outlen = inlen * 2;
    outlen = max(outlen, OK_MP_LEN * 3);
    out = (unsigned char *)kmalloc(outlen, GFP_KERNEL);

    copy_from_user(in, (unsigned char *)arg + sizeof(unsigned char *) + sizeof(int), inlen);

    outlen -= 4;
    aes_decrypt_data_extend(in+4, inlen-4, out+4, &outlen, key);

    *(int *)out = *((int *)in);

    copy_to_user((void *)arg, out, *((int *)out)+ 4);

    kfree(out);
    kfree(in);

    return OK_SUCCESS;
}
OK_RESULT ok_create_user_wrap_rsa(unsigned long __user arg)
{
    rsa_key *pa;
    copy_from_user(&pa, (void *)arg, 4);

    int len;
    copy_from_user(&len, (void *)((unsigned char *)arg+4), 4);
    char *tmp= (char *)kmalloc(len+9, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+8);
    tmp[len+8] = '\0';

    char *filename = tmp + 8;

    rsa_key * user_key;
    ok_create_key(&user_key);

    unsigned char *blob = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(user_key->N)) * 20, GFP_KERNEL);

    ok_encrypt_rsa_by_rsa(user_key, pa, blob);

    ok_store_blob(blob, filename);

    ok_free_rsa_key(&user_key);

    kfree(tmp);
    kfree(blob);
    tmp = NULL;

    return OK_SUCCESS;
}
OK_RESULT ok_load_user_wrap_rsa(unsigned long __user arg)
{
    rsa_key *pa;
    copy_from_user(&pa, (void *)arg, 4);

    int len;
    copy_from_user(&len, (void *)((unsigned char *)arg+4), 4);
    char *tmp= (char *)kmalloc(len+9, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+8);
    tmp[len+8] = '\0';

    char *filename = tmp + 8;

    rsa_key * user_key;

    ok_create_key(&user_key);

    unsigned char *blob = (unsigned char *)kmalloc(mp_unsigned_bin_size(&(user_key->N)) * 20, GFP_KERNEL);
    ok_load_blob(blob, filename);

    ok_decrypt_rsa_by_rsa(user_key, pa, blob);

    copy_to_user((void *)arg, &user_key, sizeof(rsa_key *));

    kfree(blob);
    kfree(tmp);

    return OK_SUCCESS;
}
OK_RESULT ok_create_user_wrap_aes(unsigned long __user arg)
{
    rsa_key *pa;
    copy_from_user(&pa, (void *)arg, 4);

    int len;
    copy_from_user(&len, (void *)((unsigned char *)arg+4), 4);
    char *tmp= (char *)kmalloc(len+9, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+8);
    tmp[len+8] = '\0';

    char *filename = tmp + 8;

    unsigned char key[AES_KEY_LEN];
    get_random_bytes(key, AES_KEY_LEN);

    unsigned char *blob = (unsigned char *)kmalloc(OK_MP_LEN * 3, GFP_KERNEL);

    ok_encrypt_aes_by_rsa(key, pa, blob);

    ok_store_blob(blob, filename);

    kfree(tmp);
    kfree(blob);
    tmp = NULL;

    return OK_SUCCESS;
}
OK_RESULT ok_load_user_wrap_aes(unsigned long __user arg)
{
    rsa_key *pa;
    copy_from_user(&pa, (void *)arg, 4);

    int len;
    copy_from_user(&len, (void *)((unsigned char *)arg+4), 4);
    char *tmp= (char *)kmalloc(len+9, GFP_KERNEL);
    copy_from_user(tmp, (void *)(arg), len+8);
    tmp[len+8] = '\0';

    char *filename = tmp + 8;

    unsigned char *aes_key = (unsigned char *)kmalloc(AES_KEY_LEN, GFP_KERNEL);

    unsigned char *blob = (unsigned char *)kmalloc(OK_MP_LEN * 3, GFP_KERNEL);
    ok_load_blob(blob, filename);

    ok_decrypt_aes_by_rsa(aes_key, pa, blob);

    copy_to_user((void *)arg, &aes_key, sizeof(unsigned char *));

    kfree(blob);
    kfree(tmp);

    return OK_SUCCESS;
}

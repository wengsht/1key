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

    
    /* 
    char tmp2[128] = "wengsht v6";
    len = 11;
    char out[128];
    int outlen = 128;
    rsa_encrypt_data(tmp2, len, out, &outlen, user_key);
    len = 128;
    tmp2[0] = '0';
    rsa_decrypt_data(out, outlen, tmp2, &len, user_key);
    printk("de test \n");
    printk("%s\n", tmp2);

    printk("xxd %p\n", user_key);
    */

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

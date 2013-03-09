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
OK_RESULT ok_store_srk(rsa_key *key)
{
    unsigned char *tmp = (unsigned char *) kmalloc(sizeof(mp_digit) * (key->N).used + sizeof(int) * 4, GFP_KERNEL);

    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    fp = filp_open("/root/SRK_KEY_FILE", O_RDWR | O_CREAT, 0644);

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

OK_RESULT ok_load_srk(rsa_key **key)
{
    ok_free_rsa_key(key);
    ok_create_key(key);

    unsigned char *tmp = (unsigned char *) kmalloc(sizeof(mp_digit) * MP_PREC + sizeof(int) * 3, GFP_KERNEL);

    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    fp = filp_open("/root/SRK_KEY_FILE", O_RDWR, 0644);

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

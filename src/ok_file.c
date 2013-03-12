/*
 * =====================================================================================
 *
 *       Filename:  ok_file.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/13/2013 10:35:51 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ok.h"

#include "tomcrypt.h"

loff_t ok_llseek(struct file *filp, loff_t loff, int whence)
{
    struct ok_dev * ok_devp = filp->private_data;
    loff_t ret = 0;
    switch(whence)
    {
        case 0: /*  SEEK_SET */
            ret = loff;
            break;
        case 1: /*  SEEK_CUR */
            ret = filp->f_pos + loff;
            break;
        case 2: /*  SEEK_END */
            ret = ok_devp->size + loff;
            break;
        default:
            return -EINVAL;
    }
    if(ret < 0) return -EINVAL;

    filp->f_pos = ret;
    return 0;
}
ssize_t ok_read(struct file * filp, char __user * userp, size_t count, loff_t *lofp)
{
    struct ok_dev *devp = filp->private_data;
    if(!devp)
    {
        OKDEBUG( "ok_read get ok_dev failed!\n");
        goto out;
    }
    int quantum = devp->quantum, qset = devp->qset;
    struct ok_qset *datap;
    int itemsize = quantum * qset;

    int recnt = 0, round_cnt = 0;

    int qset_no = 0, rest_size = 0;
    int quantum_no = 0, quantum_off = 0;

    void *tmp;

    if(down_interruptible(&devp->sem))
        return -ERESTARTSYS;


    if(*lofp >= devp->size)
    {
        count = 0;
        goto out;
    }
    if(*lofp + count >= devp->size)
        count = devp->size - (*lofp);

    qset_no = div_u64_rem(*lofp, (unsigned int)itemsize, &rest_size);

    datap = ok_get_qsetp(devp, qset_no);

    quantum_no = rest_size / quantum;
    quantum_off = rest_size % quantum;

    tmp = kmalloc(count, GFP_KERNEL);

    if(!tmp)
    {
        OKDEBUG( "ok_read kmalloc failed !\n");

        goto out;
    }

goon_copy:
    round_cnt = quantum - quantum_off;
    if(round_cnt > count - recnt)
        round_cnt = count - recnt;
    memcpy(tmp+recnt, datap->data[quantum_no] + quantum_off, round_cnt);

    quantum_off = 0;
    quantum_no ++;
    if(quantum_no >= OK_QUANTUM)
    {
        quantum_no = 0;
        datap = datap->next;
    }

    recnt += round_cnt;

    if(recnt < count)
        goto goon_copy;
    if(copy_to_user(userp, tmp, count))
    {
        count = -EFAULT;
        goto out2;
    }
    *lofp += count;
    kfree(tmp);

out2:
    tmp = NULL;
out:
    up(&devp->sem);

    return count;
}

ssize_t ok_write(struct file * filp, const char __user * userp, size_t count, loff_t* lofp)
{
    struct ok_dev *devp = filp->private_data;
    if(!devp)
    {
        OKDEBUG( "ok_read get ok_dev failed!\n");
        goto out;
    }
    int quantum = devp->quantum, qset = devp->qset;
    struct ok_qset *datap = NULL;
    int itemsize = quantum * qset;

    int recnt = 0, round_cnt = 0;

    int qset_no = 0, rest_size = 0;
    int quantum_no = 0, quantum_off = 0;

    if(down_interruptible(&devp->sem))
    {
        count = 0;
        return -ERESTARTSYS;
    }

    qset_no = div_u64_rem(*lofp, (unsigned int)itemsize, &rest_size);

    datap = ok_get_qsetp(devp, qset_no);


    quantum_no = rest_size / quantum;
    quantum_off = rest_size % quantum;

goon_copy:
    if(!datap)
    {
        OKDEBUG( "ok_write init ok_qset failed\n");
        goto out;
    }
    if(!(datap->data))
    {
        datap->data = ok_alloc_data(); //kmalloc(sizeof(void *) * qset, GFP_KERNEL);
//        memset(datap->data, 0, sizeof(void *) * qset);

        if(!datap->data)
        {
            OKDEBUG( "ok_write init datap->data failed\n");

            goto out;
        }
    }
    if(!(datap->data[quantum_no]))
    {
        datap->data[quantum_no] = ok_alloc_quantum(); //kmalloc(quantum * sizeof(char), GFP_KERNEL);

        if(!(datap->data[quantum_no]))
        {
            OKDEBUG( "ok_write init datap->data[] failed\n");

            goto out;
        }
    }

    round_cnt = min(quantum-quantum_off, count-recnt);
    copy_from_user(datap->data[quantum_no] + quantum_off, userp+recnt, round_cnt);

    recnt += round_cnt;
    quantum_off = 0;
    quantum_no++;

    if(quantum_no >= OK_QSET)
    {
        quantum_no = 0;

        qset_no ++;

        datap = ok_get_qsetp(devp, qset_no);
    }

    if(recnt < count)
        goto goon_copy;

out:
    count = recnt;
    *lofp += count;
    if(devp->size < *lofp)
        devp->size = *lofp;

    up(&devp->sem);
    return count;
}
static void test()
{
    char data[8] = "abcdefg";
    u32 len = 7;
    u8 hash[16];

    u32 size = 0;

    struct shash_desc * sdescmd5;

    struct crypto_shash * md5 = crypto_alloc_shash("md5", 0, 0);
    if(IS_ERR(md5))
        return ;
    size = sizeof(struct shash_desc) + crypto_shash_descsize(md5);
    sdescmd5 = kmalloc(size, GFP_KERNEL);

    sdescmd5->tfm = md5;
    sdescmd5->flags = 0x0;
    crypto_shash_init(sdescmd5);

    crypto_shash_update(sdescmd5, data, len);
    crypto_shash_final(sdescmd5, hash);

    output_hex(hash, 16);
    kfree(sdescmd5);

    crypto_free_shash(md5);
}
static void test1()
{
    rsa_key *key;

    key = (rsa_key *)kmalloc(sizeof(rsa_key), GFP_KERNEL);
    rsa_make_key(128, 65537, key);

    unsigned char msg[1024] = "wengshtv5";
    unsigned long msglen = 9;
    int block_type = LTC_LTC_PKCS_1_EME;
    unsigned long modulus_bitlen = mp_count_bits(&(key->N));
    unsigned char out[1024];
    unsigned long outlen = 1024;

    pkcs_1_v1_5_encode(msg, msglen, block_type, modulus_bitlen, out, &outlen);

    msglen = 99;
    int is_valid;
    pkcs_1_v1_5_decode(out, outlen, block_type,modulus_bitlen, msg, &msglen, &is_valid);

    msg[20] = '\0';
    printk("%s\n", msg);


    rsa_free(key);
    kfree(key);
}
void test2()
{
    struct file *fp;
    mm_segment_t fs;
    loff_t pos;
    fp = filp_open("/root/1.txt", O_RDWR | O_CREAT, 0644);

    char buf[] = "wengsht";
    if(IS_ERR(fp))
    {
        OKDEBUG("open file error\n");

        return ;
    }
    fs = get_fs();
    set_fs(KERNEL_DS);
    pos = 0;
    vfs_write(fp, buf, sizeof(buf), &pos);
    buf[0] = '0';
    pos = 0;
    vfs_read(fp, buf, sizeof(buf), &pos);
    printk("%s\n", buf);
    filp_close(fp, NULL);
    set_fs(fs);
    return ;
}
long ok_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    switch((cmd))
    {
        case OK_VERSION:
            ok_print_version(arg);
            break;
        case OK_SRK_CREATE:
            ok_create_srk(&srk_key);

            break;
        case OK_SRK_LOAD:
            ok_load_srk(&srk_key);

            break;

        case OK_USER_RSA_CREATE:
            ok_create_user_rsa(arg);

            break;
        case OK_USER_RSA_LOAD:
            ok_load_user_rsa(arg);
            break;


        case OK_CLEAR:
            break;

            

        default:
            return -ENOTTY; 
    }
    return 0;
}
long ok_compat_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    return 0;
}
int ok_open(struct inode *inodp, struct file *filp)
{
    struct ok_dev *ok_devp;
    ok_devp = container_of(inodp->i_cdev, struct ok_dev, cdev);
    filp->private_data = ok_devp;
    if(!ok_devp)
    {
        OKDEBUG( "ok_read get ok_dev failed!\n");
    }

    if((filp->f_flags & O_ACCMODE) == O_WRONLY)
    {
//        if(down_interruptible(&ok_devp->sem))
//            return -ERESTARTSYS;
//        ok_trim(ok_devp);
//        up(&ok_devp->sem);
    }
    OKDEBUG( "ok_open done\n");

    return 0;
}
int ok_release(struct inode *inodp, struct file *filp)
{
//    struct ok_dev *ok_devp;
//    ok_devp = container_of(inodp->i_cdev, struct ok_dev, cdev);
    
//    ok_trim(ok_devp);
    return 0;
}


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
#include "ok_file.h"
#include "ok_type.h"
#include "ok_mem.h"
#include <linux/kernel.h>    // container_of
#include <linux/slab.h>
#include <asm/uaccess.h> //copy_to_user copy_from_user
#include <linux/errno.h>


loff_t ok_llseek(struct file *filp, loff_t loff, int n)
{
    return 0;
}
ssize_t ok_read(struct file * filp, char __user * userp, size_t count, loff_t *lofp)
{
    struct ok_dev *devp = filp->private_data;
    if(!devp)
    {
        printk(KERN_NOTICE "ok_read get ok_dev failed!\n");
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
        printk(KERN_NOTICE "ok_read kmalloc failed !\n");

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

    printk("%d read\n", count);
    return count;
}

ssize_t ok_write(struct file * filp, const char __user * userp, size_t count, loff_t* lofp)
{
    struct ok_dev *devp = filp->private_data;
    if(!devp)
    {
        printk(KERN_NOTICE "ok_read get ok_dev failed!\n");
        goto out;
    }
    printk("ok_write %lld %d %d\n", *lofp, devp->size, count);
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
        printk(KERN_NOTICE "ok_write init ok_qset failed\n");
        goto out;
    }
    if(!(datap->data))
    {
        datap->data = ok_alloc_data(); //kmalloc(sizeof(void *) * qset, GFP_KERNEL);
//        memset(datap->data, 0, sizeof(void *) * qset);

        if(!datap->data)
        {
            printk(KERN_DEBUG "ok_write init datap->data failed\n");

            goto out;
        }
    }
    if(!(datap->data[quantum_no]))
    {
        datap->data[quantum_no] = ok_alloc_quantum(); //kmalloc(quantum * sizeof(char), GFP_KERNEL);

        if(!(datap->data[quantum_no]))
        {
            printk(KERN_DEBUG "ok_write init datap->data[] failed\n");

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
long ok_unlocked_ioctl(struct file *filp, unsigned int n, unsigned long m)
{
    return 0;
}
long ok_compat_ioctl(struct file *filp, unsigned int n, unsigned long m)
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
        printk(KERN_NOTICE "ok_read get ok_dev failed!\n");
    }

    if((filp->f_flags & O_ACCMODE) == O_WRONLY)
    {
//        if(down_interruptible(&ok_devp->sem))
//            return -ERESTARTSYS;
//        ok_trim(ok_devp);
//        up(&ok_devp->sem);
    }
    printk(KERN_NOTICE "ok_open done\n");

    return 0;
}
int ok_release(struct inode *inodp, struct file *filp)
{
//    struct ok_dev *ok_devp;
//    ok_devp = container_of(inodp->i_cdev, struct ok_dev, cdev);
    
//    ok_trim(ok_devp);
    return 0;
}


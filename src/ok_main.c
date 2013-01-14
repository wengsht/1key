
/*
 * =====================================================================================
 *
 *       Filename:  ok_crypto.c
 *
 *    Description:  build a module to solve the request from user mode
 *
 *        Version:  1.0
 *        Created:  01/12/2013 10:20:37 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>   // file
#include <linux/cdev.h>   //cdev
#include "ok_type.h"
#include "ok_main.h"
//#include ok_file.h"
#include "ok_file.h"
#include <linux/slab.h>   //kmalloc
#include "ok_mem.h"

MODULE_LICENSE("Dual BSD/GPL");
static dev_t devno;
static int ok_major = 0, ok_minor = 0;
static char dev_name[] = "ok_crypto";

module_param(ok_major, int, S_IRUGO);

struct file_operations ok_fops = 
{
    .owner  = THIS_MODULE,
    .llseek = ok_llseek,
    .read   = ok_read,
    .write  = ok_write,
    .unlocked_ioctl = ok_unlocked_ioctl,
    .compat_ioctl  = ok_compat_ioctl,
    .open   = ok_open,
    .release = ok_release,
};

struct ok_dev *ok_devs;

static OK_RESULT ok_setup_cdev(struct ok_dev *dev, int dev_index)
{
    printk("2: %p\n", dev);
    OK_RESULT result;
    dev_t tmp_devno = MKDEV(ok_major, ok_minor+dev_index);

    cdev_init(&dev->cdev, &ok_fops);

    sema_init(&dev->sem, 1);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &ok_fops;
    dev->quantum = OK_QUANTUM;
    dev->qset = OK_QSET;
    dev->size = 0;
    dev->data = NULL;
    result = cdev_add(&dev->cdev, tmp_devno, 1);
    if(result != OK_SUCCESS)
        printk(KERN_NOTICE "Error init char dev !\n");
    
    return result;
}
static OK_RESULT ok_cleanup(void)
{
    int i;
    dev_t tmp_devno = MKDEV(ok_major, ok_minor);

    unregister_chrdev_region(tmp_devno, OK_NR_DEVS);
    printk(KERN_NOTICE "unregister_chrdev_region done !\n");

    if(ok_devs)
    {
        printk(KERN_NOTICE "freeing ok_devs\n");
        for(i = 0;i < OK_NR_MINOR;i++)
            ok_trim(&ok_devs[i]);

        for(i = 0;i < OK_NR_MINOR;i++)
            cdev_del(&(ok_devs[i].cdev));

        kfree(ok_devs);
        ok_devs = NULL;
    }

    return OK_SUCCESS;
}
static OK_RESULT init_devices(void)
{
    OK_RESULT result = OK_SUCCESS;
    int i;
    if(ok_major)
    {
        devno = MKDEV(ok_major, ok_minor);
        result = register_chrdev_region(devno, OK_NR_DEVS, dev_name);
    }
    else
    {
        result = alloc_chrdev_region(&devno, ok_minor, OK_NR_DEVS, dev_name);

        ok_major = MAJOR(devno);
    }

    if(result < 0)
    {
        printk(KERN_WARNING "ok_crypto region init failed!\n");

        goto fail;
    }

    ok_devs = kmalloc(OK_NR_MINOR * sizeof(struct ok_dev), GFP_KERNEL);

    if(!ok_devs)
    {
        printk(KERN_WARNING "ok_crypto init ok_devs kmalloc error\n");
        result = -ENOMEM;
        ok_devs = NULL;

        goto fail;
    }

    for(i = 0;i < OK_NR_MINOR;i++)
    {
        result = ok_setup_cdev(&ok_devs[i], i);
        
        if(OK_SUCCESS != result)
        {
            printk(KERN_WARNING "ok_crypto device init failed!\n");

            goto fail;
        }
    }

    return result;

fail:
    ok_cleanup();
    return result;
}
static int ok_crypto_init(void)
{
    OK_RESULT result;
    printk(KERN_ALERT "One key crypto module starting!\n");

    result = init_devices();

    if(result != OK_SUCCESS)
    {
        printk(KERN_WARNING "ok_crypto init failed!\n");

        goto out;
    }

    printk(KERN_NOTICE "One key crypto module start done\n");
out:
    return result;
}
static void ok_crypto_exit(void)
{
    printk(KERN_ALERT "One key crypto module exiting!\n");

    ok_cleanup();
}
module_init(ok_crypto_init);
module_exit(ok_crypto_exit);


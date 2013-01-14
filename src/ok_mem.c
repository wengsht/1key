/*
 * =====================================================================================
 *
 *       Filename:  ok_mem.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/14/2013 02:03:14 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ok_mem.h"
#include <linux/slab.h>

OK_RESULT ok_trim(struct ok_dev *ok_devp)
{
    struct ok_qset *next, *now;
    int qset = ok_devp->qset;

    int i;
    for(now = ok_devp->data; now != NULL; now = next)
    {
        if(now->data)
        {
            for(i = 0;i < qset;i++)
            {
                kfree(now->data[i]);
            }
            kfree(now->data);
            now->data = NULL;
        }
        next = now->next;
        kfree(now);
    }
    ok_devp->size = 0;
    ok_devp->qset = OK_QSET;
    ok_devp->quantum = OK_QUANTUM;
    ok_devp->data = NULL;
    printk(KERN_DEBUG "ok_trim success\n");

    return OK_SUCCESS;
}
struct ok_qset * ok_alloc_qsetp(void)
{
    struct ok_qset *ret = NULL;
    ret = kmalloc(sizeof(struct ok_qset), GFP_KERNEL);
    ret->data = NULL;
    ret->next = NULL;

    if(!ret)
    {
        printk(KERN_DEBUG "ok_qset alloc failed\n");
        return ret;
    }
    return ret;
}
void ** ok_alloc_data()
{
    void **ret;

    ret = kmalloc(sizeof(void *) * OK_QSET, GFP_KERNEL);
    if(!ret)
    {
        printk(KERN_DEBUG "ok_data alloc failed\n");
        return ret;
    }
    memset(ret, 0, sizeof(void *) * OK_QSET);
    return ret;
}
char * ok_alloc_quantum(void)
{
    char *ret;
    ret = kmalloc(OK_QUANTUM * sizeof(char), GFP_KERNEL);

    if(!ret)
    {
        printk(KERN_DEBUG "ok_quantum alloc failed\n");

        return ret;
    }
    return ret;
}
struct ok_qset * ok_get_qsetp(struct ok_dev* devp, int qset_no)
{
    struct ok_qset *ret = devp->data;

    if(!ret)
    {
        printk(KERN_DEBUG "ok_qset setting\n");
        devp->data = ok_alloc_qsetp(); //kmalloc(sizeof(struct ok_qset), GFP_KERNEL);
        ret = devp->data;
    }
    if(!ret)
    {
        printk(KERN_DEBUG "ok_qset failed\n");
        return ret;
    }

    while(qset_no--)
    {
        if(!(ret->next))
        {
            ret->next = ok_alloc_qsetp(); //kmalloc(sizeof(struct ok_qset), GFP_KERNEL);
        }
        ret = ret->next;
    }

    return ret;
}

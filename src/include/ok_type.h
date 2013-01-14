/*
 * =====================================================================================
 *
 *       Filename:  ok_type.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/13/2013 09:37:41 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_TYPE_H__
#define __OK_TYPE_H__
#include <linux/cdev.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

typedef unsigned int OK_RESULT;

#define OK_SUCCESS 0

#define OK_QSET 128
#define OK_QUANTUM 1024
#define OK_NR_DEVS 1
#define OK_NR_MINOR 4

struct ok_qset
{
    void **data;
    struct ok_qset *next;
};
struct ok_dev
{
    struct ok_qset *data;
    int quantum;
    int qset;
    unsigned long size;
    unsigned int access_key;
    struct semaphore sem;
    struct cdev cdev;
};
#endif

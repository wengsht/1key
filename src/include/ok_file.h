/*
 * =====================================================================================
 *
 *       Filename:  ok_file.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/13/2013 10:35:59 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_FILE_H__
#define __OK_FILE_H__

#include "ok.h"
#include "ok_type.h"
#include "ok_file.h"
#include "ok_mem.h"

loff_t ok_llseek(struct file *, loff_t, int);
ssize_t ok_read(struct file *, char __user *, size_t, loff_t *);
ssize_t ok_write(struct file *, const char __user *, size_t, loff_t *);
long ok_unlocked_ioctl(struct file *, unsigned int, unsigned long);
long ok_compat_ioctl(struct file *, unsigned int, unsigned long);
int ok_open(struct inode *, struct file *);
int ok_release(struct inode *, struct file *);

#endif


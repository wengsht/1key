/*
 * =====================================================================================
 *
 *       Filename:  ok.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 10:23:56 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __OK_H_
#define __OK_H_

#include <linux/init.h>
#include <linux/kernel.h>    // container_of
#include <linux/errno.h>
#include <crypto/internal/hash.h>
#include <crypto/md5.h>
#include <linux/module.h>
#include <linux/fs.h>   // file
#include <linux/cdev.h>   //cdev
#include <linux/types.h>
#include <linux/cryptohash.h>
#include <linux/slab.h>   //kmalloc
#include <linux/string.h>
#include <asm/byteorder.h>
#include <linux/types.h>

#include <asm/uaccess.h>  // copy_to_user
#include <ok_file.h>
#include <ok_log.h>
#include <ok_main.h>
#include <ok_mem.h>
#include <ok_test.h>
#include <ok_api.h>
#include <ok_struct.h>
#include <ok_const.h>
#include <ok_type.h>



#endif

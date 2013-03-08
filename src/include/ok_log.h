/*
 * =====================================================================================
 *
 *       Filename:  ok_log.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/14/2013 07:43:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_LOG_H__
#define __OK_LOG_H__

#ifdef OK_DEBUG
#ifdef __KERNEL__
#define OKDEBUG(fmt,args...) printk( KERN_DEBUG "1key: " fmt, ## args)
#else
#define OKDEBUG(fmt,args...) printf(fmt, ## args)
#endif
#else
#define OKDEBUG(fmt,args...)
#endif

#endif


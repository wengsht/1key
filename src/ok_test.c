/*
 * =====================================================================================
 *
 *       Filename:  ok_test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  02/28/2013 11:04:45 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include "ok_test.h"
#include <linux/kernel.h>

void output_hex(u8 *data, u32 len)
{
    int i;
    for(i = 0;i < len;i++)
    {
        printk("%x ", data[i]);
        if(i && i % 16 == 0)
            printk("\n");
    }
    printk("\n");
}

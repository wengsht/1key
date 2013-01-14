/*
 * =====================================================================================
 *
 *       Filename:  ok_mem.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/14/2013 02:02:45 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __OK_MEM_H__
#define __OK_MEM_H__

#include "ok_type.h"

OK_RESULT ok_trim(struct ok_dev *ok_devp);
struct ok_qset * ok_alloc_qsetp(void);
void ** ok_alloc_data(void);
char * ok_alloc_quantum(void);
struct ok_qset * ok_get_qsetp(struct ok_dev* devp, int qset_no);

#endif


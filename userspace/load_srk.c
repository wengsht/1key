/*
 * =====================================================================================
 *
 *       Filename:  create_srk.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  01/15/2013 08:33:18 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>

#include "ok_const.h"
#include "ok_api_const.h"
#include "ok_api_all.h"

int main()
{
    struct OK_CONTEXT ok_context;

    printf("## Creating Context ##\n");
    Ok_Create_Context(&ok_context);

    printf("## Loading SRK##\n");
    Ok_Load_Srk(&ok_context);
    printf("## Load SRK Done!##\n");
    
    printf("## Freeing Context ##\n");
    Ok_Free_Context(&ok_context);

    return 0;
}


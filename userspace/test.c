/*
 * =====================================================================================
 *
 *       Filename:  test.c
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
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#define OK_MAX_VERSION_NAME_LEN 25
struct Ok_version
{
    char ok_name[OK_MAX_VERSION_NAME_LEN];
    unsigned int ok_version_id;
};

#define OK_MAGIC 'o'
#define OK_VERSION _IOR(OK_MAGIC, 1, struct Ok_version)
#define OK_TEST2 _IOW(OK_MAGIC, 2, struct A)
int main()
{
    struct Ok_version a;
    int f = open("/dev/ok_crypto0", O_RDONLY);
    ioctl(f, OK_VERSION, &a);
    printf("%s\n", a.ok_name);

    return 0;
}


#include <stdlib.h>
#include <string.h>

#include "ok_const.h"
#include "ok_api_const.h"
#include "ok_api_all.h"
#include "ok_log.h"

#define BOB_ROOT_FILE "/home/wengsht/bob_root_key.file"
#define BOB_AES_FILE  "/home/wengsht/bob_aes_key.file"

#define DEFAULT_KEY   "123456"

int main()
{
    struct OK_CONTEXT ok_context;
    OK_RESULT result = OK_SUCCESS;
    OK_KEY bob_root_handle, bob_aes_handle;

    result = Ok_Create_Context(&ok_context);
    if(OK_SUCCESS != result)
    {
        OKDEBUG("创建设备上下文失败!\n");
        goto FAILED;
    }
    OKDEBUG("创建设备上下文成功!\n");

    Ok_Print_Version(&ok_context);

    result = Ok_Create_User_Rsa(&ok_context, BOB_ROOT_FILE, DEFAULT_KEY, strlen(DEFAULT_KEY));
    if(OK_SUCCESS != result)
    {
        OKDEBUG("创建用户根密钥失败!\n");
        goto FAILED;
    }
    OKDEBUG("创建用户根密钥成功!\n");

    result = Ok_Load_User_Rsa(&ok_context, BOB_ROOT_FILE, DEFAULT_KEY, strlen(DEFAULT_KEY), &bob_root_handle);
    if(OK_SUCCESS != result)
    {
        OKDEBUG("加载用户根密钥失败!\n");
        goto FAILED;
    }
    OKDEBUG("加载用户根密钥成功! 句柄为: %p\n", bob_root_handle);

    result = Ok_Create_User_Wrap_Aes(&ok_context, bob_root_handle, BOB_AES_FILE);
    if(OK_SUCCESS != result)
    {
        OKDEBUG("创建普通AES密钥失败!\n");
        goto FAILED;
    }
    OKDEBUG("创建普通AES密钥成功!\n");

    result = Ok_Load_User_Wrap_Aes(&ok_context, bob_root_handle, BOB_AES_FILE, &bob_aes_handle);
    if(OK_SUCCESS != result)
    {
        OKDEBUG("加载普通AES密钥失败!\n");
        goto FAILED;
    }
    OKDEBUG("加载普通AES密钥成功! 句柄为: %p\n", bob_aes_handle);

    char data[] = "This is text to test the Install of 1Key";
    int  data_len = strlen(data) + 1;

    unsigned char out[1024];
    int outlen = 1024;

    char decrypted_data[1024];
    int  decrypted_len = 1024;

    OKDEBUG("\n开始测试AES功能，测试数据为：%s\n", data);
    result = Ok_Encrypt_User_Data_By_Aes(&ok_context, data, data_len, out, &outlen, bob_aes_handle);
    if(OK_SUCCESS != result)
    {
        OKDEBUG("使用AES加密服务失败!\n");
        goto FAILED;
    }
    OKDEBUG("使用AES加密服务成功!\n");

    result = Ok_Decrypt_User_Data_By_Aes(&ok_context, out, outlen, decrypted_data, &decrypted_len, bob_aes_handle);
    if(OK_SUCCESS != result)
    {
        OKDEBUG("使用AES解密服务失败!\n");
        goto FAILED;
    }
    OKDEBUG("使用AES解密服务成功!\n");

    OKDEBUG("\n测试AES功能完成，解密后数据为：%s\n", data);

FAILED:
    Ok_Free_Context(&ok_context);

    return 0;
}


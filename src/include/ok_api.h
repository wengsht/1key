/*
 * =====================================================================================
 *
 *       Filename:  ok_api.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  03/08/2013 10:36:40 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  wengsht (), wengsht.sysu@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#ifndef __OK_API_H_
#define __OK_API_H_

#include "ok.h"
#include "tomcrypt.h"
OK_RESULT ok_print_version(unsigned long arg);

OK_RESULT ok_store_rsa_key(rsa_key *key, char *file_name);
OK_RESULT ok_store_srk(rsa_key *key);

OK_RESULT ok_create_key(rsa_key **key);

OK_RESULT ok_create_user_rsa(unsigned long __user arg);
OK_RESULT ok_load_user_rsa(unsigned long __user arg);
OK_RESULT ok_create_srk(rsa_key **key);

OK_RESULT ok_load_rsa_key(rsa_key **key, char *file_name);
OK_RESULT ok_load_srk(rsa_key **key);
OK_RESULT ok_free_rsa_key(rsa_key **key);

OK_RESULT ok_rsa_encrypt_user_data(unsigned long arg);
OK_RESULT ok_rsa_decrypt_user_data(unsigned long arg);
OK_RESULT ok_make_user_hash(unsigned long arg);

OK_RESULT ok_sign_user_hash(unsigned long arg);

OK_RESULT ok_verify_user_hash(unsigned long arg);

OK_RESULT ok_create_user_aes(unsigned long __user arg);

OK_RESULT ok_load_user_aes(unsigned long __user arg);

OK_RESULT ok_aes_encrypt_user_data(unsigned long arg);
OK_RESULT ok_aes_decrypt_user_data(unsigned long arg);
OK_RESULT ok_create_user_wrap_rsa(unsigned long __user arg);
OK_RESULT ok_load_user_wrap_rsa(unsigned long __user arg);
OK_RESULT ok_create_user_wrap_aes(unsigned long __user arg);
OK_RESULT ok_load_user_wrap_aes(unsigned long __user arg);
OK_RESULT ok_encrypt_rsa_by_rsa(rsa_key *in, rsa_key *fa, unsigned char * blob);
OK_RESULT ok_decrypt_rsa_by_rsa(rsa_key *in,rsa_key *fa, unsigned char *blob);
OK_RESULT ok_encrypt_aes_by_rsa(unsigned char * aes_key,rsa_key *fa, unsigned char *blob);
OK_RESULT ok_decrypt_aes_by_rsa(unsigned char *aes_key, rsa_key *fa, unsigned char *blob);

#endif

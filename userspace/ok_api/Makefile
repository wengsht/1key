CFLAGS := -I../include -I../../src/include 
LIB_OK_API_NAME = libok_api.so

ALL: $(LIB_OK_API_NAME)

$(LIB_OK_API_NAME): ok_create_context.o ok_free_context.o ok_print_version.o ok_create_user_rsa.o ok_create_srk.o ok_load_srk.o ok_load_user_rsa.o ok_encrypt_user_data_by_rsa.o ok_decrypt_user_data_by_rsa.o ok_make_hash.o ok_sign_hash.o ok_verify_hash.o ok_create_user_aes.o ok_load_user_aes.o ok_encrypt_user_data_by_aes.o ok_decrypt_user_data_by_aes.o ok_create_user_wrap_aes.o ok_load_user_wrap_aes.o ok_create_user_wrap_rsa.o ok_load_user_wrap_rsa.o
	gcc -shared $(CFLAGS) $? -o $@

ok_create_context.o: ok_create_context.c
	gcc -c $? $(CFLAGS)

ok_free_context.o: ok_free_context.c
	gcc -c $? $(CFLAGS)

ok_print_version.o: ok_print_version.c
	gcc -c $? $(CFLAGS)

ok_load_srk.o: ok_load_srk.c
	gcc -c $? $(CFLAGS)

ok_create_srk.o: ok_create_srk.c
	gcc -c $? $(CFLAGS)

ok_create_user_rsa.o: ok_create_user_rsa.c 
	gcc -c $? $(CFLAGS)

ok_load_user_rsa.o: ok_load_user_rsa.c
	gcc -c $? $(CFLAGS)

ok_encrypt_user_data_by_rsa.o: ok_encrypt_user_data_by_rsa.c
	gcc -c $? $(CFLAGS)

ok_decrypt_user_data_by_rsa.o: ok_decrypt_user_data_by_rsa.c
	gcc -c $? $(CFLAGS)

ok_make_hash.o: ok_make_hash.c
	gcc -c $? $(CFLAGS)

ok_sign_hash.o:ok_sign_hash.c 
	gcc -c $? $(CFLAGS)
	
ok_verify_hash.o: ok_verify_hash.c
	gcc -c $? $(CFLAGS)

ok_create_user_aes.o: ok_create_user_aes.c
	gcc -c $? $(CFLAGS)

ok_load_user_aes.o: ok_load_user_aes.c
	gcc -c $? $(CFLAGS)

ok_encrypt_user_data_by_aes.o: ok_encrypt_user_data_by_aes.c
	gcc -c $? $(CFLAGS)

ok_decrypt_user_data_by_aes.o: ok_decrypt_user_data_by_aes.c
	gcc -c $? $(CFLAGS)

ok_create_user_wrap_aes.o: ok_create_user_wrap_aes.c
	gcc -c $? $(CFLAGS)

ok_load_user_wrap_aes.o: ok_load_user_wrap_aes.c
	gcc -c $? $(CFLAGS)

ok_create_user_wrap_rsa.o: ok_create_user_wrap_rsa.c
	gcc -c $? $(CFLAGS)

ok_load_user_wrap_rsa.o: ok_load_user_wrap_rsa.c
	gcc -c $? $(CFLAGS)

install:
	mv ${LIB_OK_API_NAME} /usr/local/lib
	@echo "###  INSTALL SUCCESS  ###"


clean:
	rm libok_api.so
	rm *.o


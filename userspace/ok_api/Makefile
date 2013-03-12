CFLAGS := -I../include -I../../src/include 
LIB_OK_API_NAME = libok_api.so

ALL: $(LIB_OK_API_NAME)

$(LIB_OK_API_NAME): ok_create_context.o ok_free_context.o ok_print_version.o ok_create_user_rsa.o ok_create_srk.o ok_load_srk.o ok_load_user_rsa.o
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

clean:
	rm libok_api.so
	rm *.o


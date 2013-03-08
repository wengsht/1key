CFLAGS := -I../include -I../../src/include 
LIB_OK_API_NAME = libok_api.so

ALL: $(LIB_OK_API_NAME)

$(LIB_OK_API_NAME): ok_create_context.o ok_free_context.o ok_print_version.o
	gcc -shared $(CFLAGS) $? -o $@

ok_create_context.o: ok_create_context.c

ok_free_context.o: ok_free_context.c

ok_print_version.o: ok_print_version.c

clean:
	rm libok_api.so
	rm *.o


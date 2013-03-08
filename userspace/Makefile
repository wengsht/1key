CFLAGS += -I../src/include -I./include
LDFLAGS += 

LIB_OK_API = ./ok_api/libok_api.so

all: test

test: $(LIB_OK_API) test.c
	gcc $? -o $@ $(CFLAGS) $(LDFLAGS)

$(LIB_OK_API):
	make -C ./ok_api

clean: 
	make -C ./ok_api clean
	rm test
	rm *.o
	

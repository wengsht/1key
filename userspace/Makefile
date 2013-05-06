DEBUG := y
CFLAGS += -I../src/include -I./include
LDFLAGS += 

ifeq ($(DEBUG), y)
	DEBFLAGS = -DOK_DEBUG 
else
	DEBFLAGS = 
endif
CFLAGS+=$(DEBFLAGS)

LIB_OK_API = ./ok_api/libok_api.so

all: create_srk load_srk bob
	echo ${DEBFLAGS}

create_srk: $(LIB_OK_API) create_srk.c
	gcc $? -o $@ $(CFLAGS) $(LDFLAGS)

load_srk: $(LIB_OK_API) load_srk.c
	gcc $? -o $@ $(CFLAGS) $(LDFLAGS)

bob: $(LIB_OK_API) bob.c
	gcc $? -o $@ $(CFLAGS) $(LDFLAGS)

$(LIB_OK_API):
	make -C ./ok_api

clean: 
	make -C ./ok_api clean
	rm test
	rm *.o
	

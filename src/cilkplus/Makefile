TARGET = cilkplus
CC = gcc-cilkplus
CCFLAGS += -I/parlab/include/cilk -std=c11
LDFLAGS += -lcilkrts -lcilk
OPT = 2

DEBUG = "y"

ifeq ($(DEBUG),"y")
	CCFLAGS += -D$(TARGET)_DEBUG=1 -D$(TARGET)
	CCFLAGS += -g -O0 -Wall -Wextra -Wuninitialized
else
	CCFLAGS += -Dset$(TARGET)_DEBUG=0 -D$(TARGET)
	CCFLAGS += -O$(OPT) -Wall
endif


CCFILES += lu_rec.c lu_tiled.c
OBJ +=  $(patsubst %.c,%.o,$(CCFILES))
EXECFILES +=  $(patsubst %.c,%.exec,$(CCFILES))
DEPENDS += $(wildcard *.h) 


all: $(EXECFILES)


%.exec:	%.o common.o 
	$(CC) $(CCFLAGS) $(LDFLAGS) $^ -o $@

%.o:	%.c
	$(CC) -c $(CCFLAGS) $< -o $@


.PHONY:	clean all indent tags
clean:
	rm -f $(OBJ) $(TARGET) $(EXECFILES)
indent:
	astyle --style=linux $(CCFILES)
tags:
	ctags -R *

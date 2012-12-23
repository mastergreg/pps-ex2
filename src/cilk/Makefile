TARGET = cilk
CC = cilkc
CCFLAGS +=
OPT = 2

DEBUG = "y"

ifeq ($(DEBUG),"y")
	CCFLAGS += -D$(TARGET)_DEBUG=1 -D$(TARGET)
	CCFLAGS += -g -O0 -Wall -Wextra -Wuninitialized
else
	CCFLAGS += -Dset$(TARGET)_DEBUG=0 -D$(TARGET)
	CCFLAGS += -O$(OPT) -Wall
endif


CCFILES += lu_rec.cilk lu_tiled.cilk
OBJ +=  $(patsubst %.cilk,%.o,$(CCFILES))
EXECFILES +=  $(patsubst %.cilk,%.exec,$(CCFILES))
DEPENDS += $(wildcard *.h) 


all: $(EXECFILES)

%.exec:	%.o common.o 
	$(CC) $(CCFLAGS) $^ -o $@

%.o:	%.cilk
	$(CC) -c $(CCFLAGS) $< -o $@


.PHONY:	clean all indent tags
clean:
	rm -f $(OBJ) $(TARGET) $(EXECFILES)
indent:
	astyle --style=linux $(CCFILES)
tags:
	ctags -R *

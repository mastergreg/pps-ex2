TARGET = main
CC = gcc
CCFLAGS +=

ifndef DEBUG
       DEBUG = n
endif

ifeq ($(DEBUG),y)
	CCFLAGS += -D$(TARGET)_DEBUG=1
	CCFLAGS += -g -O0 -Werror -Wall -Wextra -Wuninitialized
	LDFLAGS += -lefence
else
	CCFLAGS += -D$(TARGET)_DEBUG=0
	CCFLAGS += -Werror -Wall
	CCFLAGS+= -march=native -O3 -fexpensive-optimizations -funroll-loops \
		 -fmove-loop-invariants -fprefetch-loop-arrays -ftree-loop-optimize \
		 -ftree-vect-loop-version -ftree-vectorize
endif

CCFILES += $(wildcard *.c)
OBJ +=  $(patsubst %.c,%.o,$(CCFILES))
DEPENDS += $(wildcard *.h) 


all: $(TARGET).exec

$(TARGET).exec: $(OBJ) $(DEPENDS)
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET).exec

%.o:	%.c
	$(CC) -c $(LDFLAGS) $(CCFLAGS) $< -o $@


.PHONY:	clean all indent tags
clean:
	@echo "Cleaning up"
	@rm -f $(OBJ) $(TARGET) $(EXECFILES)
indent:
	astyle --style=linux $(CCFILES)
tags:
	ctags -R *

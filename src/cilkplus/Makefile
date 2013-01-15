TARGET = USE_CILKPLUS
CC = gcc-cilkplus
CCFLAGS += -std=c11
CCFLAGS += -Wall -Wextra -Wuninitialized -Wunused -fcilkplus
LDFLAGS += -Wl,-R/parlab/lib/ -lcilkrts
OPT = 3

ifndef DEBUG
	DEBUG = "y"
endif

ifeq ($(DEBUG),"y")
	CCFLAGS += -D$(TARGET)_DEBUG=1 -D$(TARGET)
	CCFLAGS += -g -O1 -pg
else
	CCFLAGS += -D$(TARGET)_DEBUG=0 -D$(TARGET)
	CCFLAGS += -O$(OPT)
endif


CCFILES += lu_rec.c lu_tiled.c
OBJ +=  $(patsubst %.c,%.o,$(CCFILES))
EXECFILES +=  $(patsubst %.c,%.exec,$(CCFILES))
DEPENDS += $(wildcard *.h)


all: $(EXECFILES)

%.exec:	%.o common.o
	$(CC) $(CCFLAGS) $^ $(LDFLAGS) -o $@

%.o:	%.c
	$(CC) $< -c $(CCFLAGS) -o $@


.PHONY:	clean all indent tags
clean:
	rm -f $(OBJ) $(TARGET) $(EXECFILES)
indent:
	astyle -A3 $(CCFILES)
tags:
	ctags -R *

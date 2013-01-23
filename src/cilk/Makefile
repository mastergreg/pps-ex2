TARGET = USE_CILK
CC = cilkc
CCFLAGS += -Wall -Wextra -Wuninitialized -Wunused
OPT = 2

ifndef DEBUG
	DEBUG = "y"
endif

ifeq ($(DEBUG),"y")
	CCFLAGS += -D$(TARGET)_DEBUG=1 -D$(TARGET)
	CCFLAGS += -g -O0 
else
	CCFLAGS += -Dset$(TARGET)_DEBUG=0 -D$(TARGET)
	CCFLAGS += -O$(OPT)
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
	@echo "Cleaning up"
	@rm -f $(OBJ) $(TARGET) $(EXECFILES)
indent:
	astyle -A3 $(CCFILES)
tags:
	ctags -R *

TARGET = main
CC = gcc
CCFLAGS +=

ifndef DEBUG
       DEBUG = n
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
	rm -f $(OBJ) $(TARGET)
indent:
	astyle --style=linux $(CCFILES)
tags:
	ctags -R *

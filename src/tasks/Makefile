TARGET = USE_CILKPLUS
CC = gcc-cilkplus
CCFLAGS += -std=c11
#CCFLAGS += -Wuninitialized -Wunused `pkg-config --cflags glib-2.0` -fcilkplus
CCFLAGS += `pkg-config --cflags glib-2.0` -fcilkplus -w -lefence
LDFLAGS += -Wl,-R/parlab/lib/ -lcilkrts  `pkg-config --libs glib-2.0` -lefence
OPT = 1

ifndef DEBUG
	DEBUG = "y"
endif

ifeq ($(DEBUG),"y")
	CCFLAGS += -Dmain_DEBUG=1 -D$(TARGET)
	CCFLAGS += -g -O0 
else
	CCFLAGS += -Dmain_DEBUG=0 -D$(TARGET)
	CCFLAGS += -O$(OPT)
endif


CCFILES += lu_tiled.c
OBJ +=  $(patsubst %.c,%.o,$(CCFILES))
EXECFILES +=  $(patsubst %.c,%.exec,$(CCFILES))
DEPENDS += $(wildcard *.h) 


all: $(EXECFILES)

%.exec:	%.o common.o lu_tiled_wrappers.o task_config.h
	@echo "[LD]" $@
	@$(CC) $(LDFLAGS) $(CCFLAGS) $^ -o $@

%.o:	%.c
	@echo "[CC]" $<
	@$(CC) -c $(LDFLAGS) $(CCFLAGS) $< -o $@


.PHONY:	clean all indent tags
clean:
	@echo "Cleaning up"
	@rm -f $(OBJ) $(TARGET) $(EXECFILES)
indent:
	astyle -A3 $(CCFILES)
tags:
	ctags -R *

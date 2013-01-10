CC=gcc
CCFLAGS += -Wall -Wextra -Wuninitialized -Wunused 


CCFILES += lu_tiled.c lu_rec.c
OBJ +=  $(patsubst %.c,%.o,$(CCFILES))
EXECFILES +=  $(patsubst %.c,%.exec,$(CCFILES))
DEPENDS += $(wildcard *.h) 

all: $(EXECFILES)

%.exec:	%.o common.o 
	$(CC) $(CCFLAGS) $^ -o $@

%.o:	%.c
	$(CC) $< -c $(CCFLAGS) -o $@

clean:
	rm -f $(OBJ) $(TARGET) $(EXECFILES)

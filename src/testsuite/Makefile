all: generator serial cilk cilkplus tasks
debug: generator-dbg serial-dbg cilk-dbg cilkplus-dbg

generator:
	make -B DEBUG="n" -C ../generator
serial:
	make -B DEBUG="n" -C ../serial
	make -B DEBUG="n" -C ../lu
cilk:
	make -B DEBUG="n" -C ../cilk
cilkplus:
	make -B DEBUG="n" -C ../cilkplus
tasks:
	make -B DEBUG="n" -C ../tasks




generator-dbg:
	make -B -C ../generator
serial-dbg:
	make -B -C ../serial
cilk-dbg:
	make -B -C ../cilk
cilkplus-dbg:
	make -B -C ../cilkplus


OUTFILES=$(wildcard *.out)
LOGFILES=$(wildcard *.log)
NEWLOGFILES=$(wildcard *.log.new)

INFILES=$(wildcard *.in)
ERRORFILES=$(wildcard *.err)

.PHONY: clean

clean:
	@echo "Cleaning up"
	@make clean -C ../generator
	@make clean -C ../serial
	@make clean -C ../cilk
	@make clean -C ../cilkplus
	@rm -f $(OUTFILES)
	@rm -f $(NEWLOGFILES)

	

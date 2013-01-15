all: generator serial cilk cilkplus
debug: generator-dbg serial-dbg cilk-dbg cilkplus-dbg

generator:
	make -B DEBUG="n" -C ../generator
serial:
	make -B DEBUG="n" -C ../serial
cilk:
	make -B DEBUG="n" -C ../cilk
cilkplus:
	make -B DEBUG="n" -C ../cilkplus




generator-dbg:
	make -B -C ../generator
serial-dbg:
	make -B -C ../serial
cilk-dbg:
	make -B -C ../cilk
cilkplus-dbg:
	make -B -C ../cilkplus

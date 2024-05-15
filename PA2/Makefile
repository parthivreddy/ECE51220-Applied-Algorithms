WARNING = --pedantic -Wvla -Wall -Wshadow
GCC = gcc -std=c99 $(WARNING) -O3 *.c -o pa2
GCCg = gcc -std=c99 $(WARNING) -g *.c -o pa2g
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full -s --verbose
TEST3 = ./pa2 examples/3.txt outputs/3.dim outputs/3.pck outputs/3_all.dim outputs/3_all.pck
TEST8 = ./pa2 examples/8.txt outputs/8.dim outputs/8.pck outputs/8_all.dim outputs/8_all.pck
TEST10 = ./pa2 examples/10_1.txt outputs/10.dim outputs/10.pck outputs/10_all.dim outputs/10_all.pck
TEST100 = ./pa2 examples/100_3.txt outputs/100.dim outputs/100.pck outputs/100_all.dim outputs/100_all.pck
TEST500 = ./pa2 examples/500_5.txt outputs/500.dim outputs/500.pck outputs/500_all.dim outputs/500_all.pck


pa2: *.c pa2.h tree.h
	$(GCC) 

pa2g: *.c pa2.h tree.h
	$(GCCg) 

testall: pa2
	$(TEST3)
	$(TEST8)
	$(TEST10)
	$(TEST100)
	$(TEST500)

testallg: pa2g
	$(TEST3)
	$(TEST8)
	$(TEST10)
	$(TEST100)
	$(TEST500)

test3: pa2
	$(TEST3)

test8: pa2
	$(TEST8)

test10: pa2
	$(TEST10)

test100: pa2
	$(TEST100)

test500: pa2
	$(TEST500)

%.o : %.c
	$(GCC) -c $<

diff3:
	diff examples/3_1.dim outputs/3.dim
	diff examples/3_1.pck outputs/3.pck
	diff examples/3_all.dim outputs/3_all.dim
	diff examples/3_all.pck outputs/3_all.pck

diff8:
	diff examples/8_1.dim outputs/8.dim
	diff examples/8_1.pck outputs/8.pck
	diff examples/8_all.dim outputs/8_all.dim
	diff examples/8_all.pck outputs/8_all.pck


memory: pa2
	$(VAL) --log-file="outputs/3.log" $(TEST3) 
	$(VAL) --log-file="outputs/8.log" $(TEST8)
	$(VAL) --log-file="outputs/10.log" $(TEST10)
	$(VAL) --log-file="outputs/100.log" $(TEST100)
	$(VAL) --log-file="outputs/500.log" $(TEST500)

clean:
	/bin/rm -f pa2
	/bin/rm -f pa2g
	/bin/rm -f outputs/*

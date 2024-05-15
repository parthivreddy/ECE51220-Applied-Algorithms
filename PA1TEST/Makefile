WARNING = -Wall -Wshadow --pedantic
ERROR = -Wvla -Wshadow
GCCg = gcc -std=c99 -g $(WARNING) $(ERROR) *.c -o pa1 -lm
GCC = gcc -std=c99 -O3 $(WARNING) $(ERROR) *.c -o pa1 -lm
DELAY = 21
VAL = valgrind --tool=memcheck --log-file=memcheck.txt --leak-check=full -s --verbose
IN3 = examples/fake_inv.param examples/fake_wire.param examples/3.txt
OUT3 = outputs/3.pre outputs/3.elmore outputs/3.ttopo outputs/3.btopo
IN = examples/inv.param examples/wire.param
OUT5 = outputs/5.pre outputs/5.elmore outputs/5.ttopo outputs/5.btopo
OUTs5378 = outputs/s5378.pre outputs/s5378.elmore outputs/s5378.ttopo outputs/s5378.btopo
OUTs1423 = outputs/s1423.pre outputs/s1423.elmore outputs/s1423.ttopo outputs/s1423.btopo
OUTp1 = outputs/p1.pre outputs/p1.elmore outputs/p1.ttopo outputs/p1.btopo
TEST5 = ./pa1 $(DELAY) $(IN) examples/5.txt $(OUT5)
TEST3 = ./pa1 $(DELAY) $(IN3) $(OUT3)
TESTs1423 = ./pa1 $(DELAY) $(IN) examples/s1423.txt $(OUTs1423)
TESTs5378 = ./pa1 $(DELAY) $(IN) examples/s5378.txt $(OUTs5378)
TESTp1 = ./pa1 $(DELAY) $(IN) examples/p1.txt $(OUTp1)


testall : pa1
	$(TEST3)
	$(TEST5)
	$(TESTs1423)
	$(TESTs5378)
	$(TESTp1)

pa1 : *.c pa1.h tree.h
	$(GCC) 

test3 : pa1
	./pa1 $(DELAY) $(IN3) $(OUT3)

test5 : pa1
	$(TEST5)

tests1423 : pa1
	$(TESTs1423)

tests5378 : pa1
	$(TESTs5378)

testp1 : pa1
	$(TESTp1)


memory : pa1
	valgrind --leak-check=full -s $(TEST3)
	valgrind --leak-check=full -s $(TEST5)
	valgrind --leak-check=full -s $(TESTs1423)
	valgrind --leak-check=full -s $(TESTs5378)
	valgrind --leak-check=full -s $(TESTp1)

clean : 
	/bin/rm -f *.o
	/bin/rm -f outputs/*
	/bin/rm -f pa1

%.o : %.c
	$(GCC) -c $<


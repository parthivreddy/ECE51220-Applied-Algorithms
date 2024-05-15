WARNING = -pedantic -Wvla -Wall -Wshadow
GCC = gcc -std=c99 $(WARNING) -O3 *.c -o pa3 -lm
GCCg = gcc -std=c99 $(WARNING) -g *.c -o pa3g
VAL = valgrind --track-origins=yes --leak-check=full -s
TEST7_8 = ./pa3 pa3_examples/7_8.gr outputs/7_8.mcm outputs/7_8.cycle
TEST10_40 = ./pa3 pa3_examples/10_40.gr outputs/10_40.mcm outputs/10_40.cycle
TEST100_400 = ./pa3 pa3_examples/100_400.gr outputs/100_400.mcm outputs/100_400.cycle
TEST200_800 = ./pa3 pa3_examples/200_800.gr outputs/200_800.mcm outputs/200_800.cycle
TEST500_2000 = ./pa3 pa3_examples/500_2000.gr outputs/500_2000.mcm outputs/500_2000.cycle
PARTHIV = ./pa3 pa3_examples/parthivTest.gr outputs/parthiv.mcm outputs/parthiv.cycle
TESTNEG = ./pa3 pa3_examples/neg7_8.gr outputs/neg7_8.mcm outputs/neg7_8.cycle


pa3: *.c graph.h pa3.h
	$(GCC)

pa3g: *.c graph.h pa3.h
	$(GCCg)

testall: pa3
	$(TEST7_8)
	$(TEST10_40)
	$(TEST100_400)
	$(TEST200_800)
	$(TEST500_2000)
	$(PARTHIV)
	$(TESTNEG)

parthiv: pa3
	$(PARTHIV)

test7_8: pa3
	$(TEST7_8)

test10_40: pa3
	$(TEST10_40)

test100_400: pa3
	$(TEST100_400)

test200_400: pa3
	$(TEST200_400)

test500_2000: pa3
	$(TEST500_2000)

memory:
	$(VAL) --log-file="outputs/7.log" $(TEST7_8)
	$(VAL) --log-file="outputs/10.log" $(TEST10_40)
	$(VAL) --log-file="outputs/100.log" $(TEST100_400)
	$(VAL) --log-file="outputs/200.log" $(TEST200_800)
	$(VAL) --log-file="outputs/500.log" $(TEST500_2000)
	$(VAL) --log-file="outputs/Parthiv.log" $(PARTHIV)
	$(VAL) --log-file="outputs/neg.log" $(TESTNEG)

%.o : %.c
	$(GCC) -c $<
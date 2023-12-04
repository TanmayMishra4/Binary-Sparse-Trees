CC=gcc
COMMON= -Wall -Wextra -Wfloat-equal -Wpedantic -Wvla -std=c99 -Werror
DEBUG= -g3
SANITIZE= $(COMMON) -fsanitize=undefined -fsanitize=address $(DEBUG)
VALGRIND= $(COMMON) $(DEBUG)
PRODUCTION= $(COMMON) -O3

all: driverbsa_s driverbsa fibmemo sieve_s isfactorial extfibmemo_s extisfact driverbsa_valgrind fibmemo_valgrind isfactorial_valgrind sieve_valgrind extfibmemo_valgrind extisfact_valgrind

driverbsa_s: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa_s -I./Alloc $(SANITIZE)

driverbsa_valgrind: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa_valgrind -I./Alloc $(VALGRIND)

driverbsa: bsa.h Alloc/specific.h Alloc/alloc.c driver.c
	$(CC) driver.c Alloc/alloc.c -o driverbsa -I./Alloc $(PRODUCTION)

fibmemo: bsa.h Alloc/specific.h Alloc/alloc.c fibmemo.c
	$(CC) fibmemo.c Alloc/alloc.c -o fibmemo -I./Alloc $(PRODUCTION)

fibmemo_valgrind: bsa.h Alloc/specific.h Alloc/alloc.c fibmemo.c
	$(CC) fibmemo.c Alloc/alloc.c -o fibmemo_valgrind -I./Alloc $(VALGRIND)

isfactorial: bsa.h Alloc/specific.h Alloc/alloc.c isfactorial.c
	$(CC) isfactorial.c Alloc/alloc.c -o isfactorial -I./Alloc $(PRODUCTION)

isfactorial_valgrind: bsa.h Alloc/specific.h Alloc/alloc.c isfactorial.c
	$(CC) isfactorial.c Alloc/alloc.c -o isfactorial_valgrind -I./Alloc $(VALGRIND)

isfactorial_profiler: bsa.h Alloc/specific.h Alloc/alloc.c isfactorial.c
	$(CC) isfactorial.c Alloc/alloc.c -o isfactorial_prof -I./Alloc $(PRODUCTION) -pg


sieve: bsa.h Alloc/specific.h Alloc/alloc.c sieve.c
	$(CC) sieve.c Alloc/alloc.c -o sieve -I./Alloc $(PRODUCTION)

sieve_s: bsa.h Alloc/specific.h Alloc/alloc.c sieve.c
	$(CC) sieve.c Alloc/alloc.c -o sieve_s -I./Alloc $(SANITIZE)

sieve_valgrind: bsa.h Alloc/specific.h Alloc/alloc.c sieve.c
	$(CC) sieve.c Alloc/alloc.c -o sieve_valgrind -I./Alloc $(PRODUCTION)

## Only if you do an extension. Uncomment in all, extfibmemo_s and run.
extfibmemo_s: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$extsieve_s(CC) fibmemo.c Extension/extension.c -o extfibmemo_s -I./Extension $(SANITIZE)

extfibmemo_valgrind: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) fibmemo.c Extension/extension.c -o extfibmemo_valgrind -I./Extension $(VALGRIND)

extsieve_valgrind: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) sieve.c Extension/extension.c -o extsieve_valgrind -I./Extension $(VALGRIND)

extfibmemo: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) fibmemo.c Extension/extension.c -o extfibmemo -I./Extension $(PRODUCTION)

extisfact: bsa.h Extension/specific.h Extension/extension.c isfactorial.c
	$(CC) isfactorial.c Extension/extension.c -o extisfact -I./Extension $(PRODUCTION)

extisfact_valgrind: bsa.h Extension/specific.h Extension/extension.c isfactorial.c
	$(CC) isfactorial.c Extension/extension.c -o extisfact_valgrind -I./Extension $(VALGRIND)

extdriver: bsa.h Extension/specific.h Extension/extension.c driver.c
	$(CC) driver.c Extension/extension.c -o extdriver -I./Extension $(PRODUCTION)

extsieve: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) sieve.c Extension/extension.c -o extsieve -I./Extension $(PRODUCTION) 

extsieve_s: bsa.h Extension/specific.h Extension/extension.c fibmemo.c
	$(CC) sieve.c Extension/extension.c -o extsieve_s -I./Extension $(SANITIZE)

run: driverbsa driverbsa_s fibmemo sieve_s isfactorial extfibmemo_s
	./driverbsa_s
	./isfactorial
	./fibmemo
	./sieve_s
	./extfibmemo_s
	./extisfact
	./extsieve
	./extsieve_s

clean:
	rm -f driverbsa_s driverbsa fibmemo extfibmemo_s isfactorial sieve_s extisfact isfactorial extsieve_s extsieve

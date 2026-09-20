CC = mpicc
CFLAGS = -O2 -Wall -Wextra
LIBS = -lm
SOURCES = main.c input.c output.c gauss_reverse_MPI.c
OBJS = main.o input.o output.o gauss_reverse_MPI.o
EXE = a.out

all : $(EXE)

%.o : %.c
	$(CC) $(CFLAGS) -c $<

$(EXE) : $(OBJS)
	$(CC) -o $@ $^ $(LIBS)

ifneq (clean, $(MAKECMDGOALS))
    -include deps.mk
endif

deps.mk: $(SOURCES)
	$(CC) -MM $^ > $@

clean:
	-rm -f $(OBJS)
	-rm -f $(EXE)
	-rm -f deps.mk

start:
	echo "mpirun -np p ./a.out n r s [filename]"

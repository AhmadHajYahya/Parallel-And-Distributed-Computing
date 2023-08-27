# Compiler and compiler flags
CC = mpicc
CFLAGS = -Wall -Wextra -O3 -Wno-unused-result -fopenmp -g

# Source files
SRCS = General.c HelperFuncs.c main.c

# Header files
HDRS = General.h HelperFuncs.h

# Object files
OBJS = $(SRCS:.c=.o)

# Output files
OUTPUT = output.txt

# Makefile rules
all: prog

prog: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) -lm -fopenmp

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@
	
run2: prog 
	mpiexec -np 2 ./prog
	
run4: prog
	mpiexec -np 4 ./prog
	
clean:
	rm -f prog $(OBJS) $(OUTPUT)


CC = mpicc
CFLAGS = -Wall -O2
LDFLAGS = -lm
SOURCES = main.c
EXECUTABLE = main
INPUT_FILE = input6
NP = 4

all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

run: $(EXECUTABLE)
	mpirun -np $(NP) ./$< $(INPUT_FILE)

clean:
	rm -f $(EXECUTABLE)

.PHONY: all run clean


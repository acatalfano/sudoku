CC=g++
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c++14
RELEASE_FLAGS=$(CFLAGS) -O3 -D NDEBUG
DEBUG_FLAGS=$(CFLAGS) -g3 -D DEBUG
PROFILE_FLAGS=$(CFLAGS) -pg -D NDEBUG
DEPS= cell.h compare.h puzzle.h solution.h
OBJ = sudoku.o solution.o puzzle.o

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

release: $(OBJ)
	$(CC) -o sudoku $^ $(RELEASE_FLAGS)

debug: $(OBJ)
	$(CC) -o sudoku $^ $(DEBUG_FLAGS)

profile: $(OBJ)
	$(CC) -o sudoku $^ $(PROFILE_FLAGS)

.PHONY: clean

clean:
	rm -f *.o *~ sudoku
PROGS = futoshiki futoshiki2
CC = g++
CFLAGS = -Wall -O2 --std=c++17
LDFLAGS = -lm

all: $(PROGS)


$(PROGS): 
	$(CC) $(CFLAGS) -o $@ $@.cc $(LDFLAGS)


.PHONY: clean
clean:
	rm -f $(PROGS)

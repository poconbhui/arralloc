PROGRAM = arralloc.test

SOURCES = arralloc.test.c arralloc.c

CC = gcc
CFLAGS = -Wall -Wextra

$(PROGRAM): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: check
check: $(PROGRAM)
	./$(PROGRAM)

.PHONY: clean
clean:
	@echo rm $(PROGRAM)
	@test -f $(PROGRAM) && rm $(PROGRAM); true

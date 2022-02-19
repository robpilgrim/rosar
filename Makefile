rosar: rosar.c
	$(CC) $(CFLAGS) -Wall -Wextra -pedantic -O3 -o $@ $<

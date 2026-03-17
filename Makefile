all:
	gcc -Wall -Wextra -o core core.c

clean:
	rm -rf core
.PHONY: all

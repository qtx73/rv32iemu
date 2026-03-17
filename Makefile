all:
	gcc -Wall -Wextra -o core core.c

test: all
	./rvtest.sh
	./rvcsrtest.sh
	./rviqtest.sh

clean:
	rm -rf core *.bin *.elf *.hex
.PHONY: all

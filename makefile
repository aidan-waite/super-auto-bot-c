CFLAGS=-Wall -fsanitize=address -g

test: engine.c engine.h main.c models.h tests.c tests.h
	gcc $(CFLAGS) main.c engine.c tests.c -o test

clean:
	rm -f test

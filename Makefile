build: main.c
	gcc main.c -o executable

run:
	./executable

clean:
	rm executable
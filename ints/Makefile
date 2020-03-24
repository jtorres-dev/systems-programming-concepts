CC = gcc -Wno-overflow

all: ints-subtraction ints-floats square

ints-subtraction: ints-subtraction.c
	$(CC) ints-subtraction.c -o ints-subtraction

ints-floats: ints-floats.c
	@echo "Compiling ints-floats.c into ints-floats using gcc."
	@$(CC)     ints-floats.c   -o ints-floats
	@echo "Done."

square: square.c
	$(CC) square.c -o square
clean:
	rm ints-floats ints-subtraction square

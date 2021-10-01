%.o: %.asm
	acme -o $@ $<

all: helloWorld.o lda_test.o

clean:
	rm -rf *.o

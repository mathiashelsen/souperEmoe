%.o: %.asm
	acme -o $@ $<

all: helloWorld.o lda_test.o lda_test2.o

clean:
	rm -rf *.o

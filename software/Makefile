%.o: %.asm
	acme -o $@ $<

all: helloWorld.o lda_test.o lda_test2.o adc_test.o int_test.o bouncingBall.o kernal.o

clean:
	rm -rf *.o

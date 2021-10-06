%.o: %.asm
	acme -o $@ $<

all: helloWorld.o lda_test.o lda_test2.o adc_test.o int_test.o

clean:
	rm -rf *.o

BIN=.

#FLAGS= -g -O0 -Wall -Wno-unused-result -std=gnu99 -pedantic
FLAGS= -O3 -Wall -Wno-unused-result -std=gnu99 -pedantic
MC920LIB    = -L../lib -lmc920 -lm 
MC920FLAGS  = -I../include 

libift:
	cd ../; $(MAKE) ; cd -\


$@.c: libmc920 $@.c 
	make
	gcc $(FLAGS) $@.c -o $(BIN)/$@ $(MC920FLAGS) $(MC920LIB) ; \

clean:	
	rm medialfilter linearfilter morphfilter diffusionfilter fftdemo histogram 









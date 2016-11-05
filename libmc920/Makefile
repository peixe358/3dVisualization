LIB=./lib
INCLUDE=./include
SRC=./src
OBJ=./obj
BIN=./bin

#FLAGS= -g  -O0 -Wall -D _DEBUG -Wno-unused-result -fPIC -std=gnu99 -pedantic
FLAGS=  -O3 -Wall -Wno-unused-result -fPIC -std=gnu99 -pedantic

libmc920: $(LIB)/libmc920.a
	echo "libmc920.a built..."

$(LIB)/libmc920.a: \
$(OBJ)/common.o \
$(OBJ)/comptime.o \
$(OBJ)/image.o \
$(OBJ)/adjacency.o \
$(OBJ)/kernel.o \
$(OBJ)/linear.o \
$(OBJ)/gqueue.o \
$(OBJ)/spectrum.o \
$(OBJ)/matriz.o \
$(OBJ)/plane.o \
$(OBJ)/interpolation.o \

	ar csr $(LIB)/libmc920.a \
$(OBJ)/common.o \
$(OBJ)/comptime.o \
$(OBJ)/image.o \
$(OBJ)/adjacency.o \
$(OBJ)/kernel.o \
$(OBJ)/linear.o \
$(OBJ)/gqueue.o \
$(OBJ)/spectrum.o \
$(OBJ)/matriz.o \
$(OBJ)/plane.o \
$(OBJ)/interpolation.o \

$(OBJ)/common.o: $(SRC)/common.c
	gcc $(FLAGS) -c $(SRC)/common.c -I$(INCLUDE) \
	-o $(OBJ)/common.o 

$(OBJ)/comptime.o: $(SRC)/comptime.c
	gcc $(FLAGS) -c $(SRC)/comptime.c -I$(INCLUDE) \
	-o $(OBJ)/comptime.o 

$(OBJ)/image.o: $(SRC)/image.c
	gcc $(FLAGS) -c $(SRC)/image.c -I$(INCLUDE) \
	-o $(OBJ)/image.o 

$(OBJ)/adjacency.o: $(SRC)/adjacency.c
	gcc $(FLAGS) -c $(SRC)/adjacency.c -I$(INCLUDE) \
	-o $(OBJ)/adjacency.o 

$(OBJ)/kernel.o: $(SRC)/kernel.c
	gcc $(FLAGS) -c $(SRC)/kernel.c -I$(INCLUDE) \
	-o $(OBJ)/kernel.o 

$(OBJ)/linear.o: $(SRC)/linear.c
	gcc $(FLAGS) -c $(SRC)/linear.c -I$(INCLUDE) \
	-o $(OBJ)/linear.o 

$(OBJ)/gqueue.o: $(SRC)/gqueue.c
	gcc $(FLAGS) -c $(SRC)/gqueue.c -I$(INCLUDE) \
	-o $(OBJ)/gqueue.o 

$(OBJ)/spectrum.o: $(SRC)/spectrum.c
	gcc $(FLAGS) -c $(SRC)/spectrum.c -I$(INCLUDE) \
	-o $(OBJ)/spectrum.o

$(OBJ)/matriz.o: $(SRC)/matriz.c
	gcc $(FLAGS) -c $(SRC)/matriz.c -I$(INCLUDE) \
	-o $(OBJ)/matriz.o 

$(OBJ)/plane.o: $(SRC)/plane.c
	gcc $(FLAGS) -c $(SRC)/plane.c -I$(INCLUDE) \
	-o $(OBJ)/plane.o  

$(OBJ)/interpolation.o: $(SRC)/interpolation.c
	gcc $(FLAGS) -c $(SRC)/interpolation.c -I$(INCLUDE) \
	-o $(OBJ)/interpolation.o 

clean: 
	rm $(LIB)/lib*.a; rm $(OBJ)/*.o; rm $(BIN)/*; 	






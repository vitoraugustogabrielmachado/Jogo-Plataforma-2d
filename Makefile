CC = gcc
CFLAGS = -Wall
LIBS = $(shell pkg-config --libs --cflags allegro-5 allegro_font-5 allegro_ttf-5 allegro_image-5 allegro_primitives-5)
TARGET = programa
SRC = main.c inicializar.c entidades.c estadoJogo.c animacao.c
OBJ = main.o inicializar.o entidades.o estadoJogo.o animacao.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET) $(LIBS)

main.o: main.c inicializar.h 
	$(CC) $(CFLAGS) -c main.c -o main.o $(LIBS)

inicializar.o: inicializar.c inicializar.h
	$(CC) $(CFLAGS) -c inicializar.c -o inicializar.o $(LIBS)

entidades.o: entidades.c entidades.h
	$(CC) $(CFLAGS) -c entidades.c -o entidades.o $(LIBS)

estadoJogo.o: estadoJogo.c estadoJogo.h
	$(CC) $(CFLAGS) -c estadoJogo.c -o estadoJogo.o $(LIBS)

animacao.o: animacao.c animacao.h
	$(CC) $(CFLAGS) -c animacao.c -o animacao.o $(LIBS)

clean:
	rm -f $(TARGET) $(OBJ)
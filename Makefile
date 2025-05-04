all: minesweeper.exe

grille.o: grille.c grille.h pion.h
	gcc -c grille.c

histoire.o: histoire.c histoire.h ui.h
	gcc -c histoire.c

main.o: main.c histoire.h ui.h pion.h grille.h
	gcc -c main.c

pion.o: pion.c ui.h pion.h grille.h
	gcc -c pion.c

ui.o: ui.c ui.h grille.h
	gcc -c ui.c

minesweeper.exe: grille.o histoire.o main.o pion.o ui.o
	gcc grille.o histoire.o main.o pion.o ui.o -lncurses -o minesweeper.exe

clean:
	rm -rf *.o minesweeper.exe
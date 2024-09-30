#ifndef CONWAY_H_INCLUDED
#define CONWAY_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define MAG "\033[35m"
#define FIN "\033[0m"
#define TODO_OK 0

//Prototipos

int ** crearTablero(unsigned int f, unsigned int c); // creo el tablero
void caPlaneador(int ** tablero, int x, int y); // creo el ca�on planeador
int contarVecinosVivos(int ** tablero, int x, int y, unsigned int f, unsigned int c); // Cuenta la cantidad de vecinos vivos, seg posicion pasada
void calcularSiguienteGeneracion(int ** tablero, int ** proxGen, unsigned int f, unsigned int c); // Calcula la siguiente generarion
void actTablero(int ** tablero, int ** proxGen, unsigned int f, unsigned int c); // Actualiza el tablero original respecto a la proxGen
int cantPoblacion(int ** tablero, unsigned int f, unsigned int c);//Cuenta la poblacion
void liberoMem(int ** tablero, int ** proxGen, unsigned int f); // Libero memoria

#endif // CONWAY_H_INCLUDED
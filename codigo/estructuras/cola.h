#ifndef COLA_H_INCLUDED
#define COLA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#define TAM_COLA 1000
#define MIN(a,b) ((a)<(b)?(a):(b))
typedef struct
{
    char cola[TAM_COLA];
    unsigned pri;
    unsigned ult;
    unsigned tDisp;
}tCola;

typedef void (*IMP)(const void* e);
void crearCola(tCola *p);
void vaciarCola(tCola *p);
int colaLlena(tCola *p, unsigned cantB);
int colaVacia(const tCola *p);
int ponerEncola(tCola *p, const void *info, unsigned cantB);
int sacarDeCola(tCola *p, void *info, unsigned cantB);
int verPri(const tCola *p, void *info, unsigned tam);
int archivoACola(tCola *p, const char *nombArch, unsigned tamElem);
void mostrarCola(tCola *p, IMP imprimir);

#endif // COLA_H_INCLUDED

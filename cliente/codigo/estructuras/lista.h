#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#endif

typedef struct sNodo
{
    void* info;
    size_t tamInfo;
    struct sNodo* sig;
} tNodo;

typedef tNodo* tLista;

void crearLista(tLista* lista);
int listaVacia(const tLista* lista);
int listaLlena(const tLista* lista, size_t tam);
void vaciarLista(tLista* lista);
int ponerAlFinal(tLista* lista, const void* elemento, size_t tam);
int sacarPrimeroLista(tLista* lista, void* elemento, size_t tam);

#endif // LISTA_H_INCLUDED

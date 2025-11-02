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
    unsigned int tamInfo;
    struct sNodo* sig;
} tNodo;

typedef tNodo* tLista;

void crearLista(tLista* lista);
int listaVacia(const tLista* lista);
int listaLlena(const tLista* lista, unsigned int tam);
void vaciarLista(tLista* lista);
int ponerAlFinal(tLista* lista, const void* elemento, unsigned int tam);
void recorrerLista(tLista* lista, void (*accion)(void* nodo));

#endif // LISTA_H_INCLUDED

#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED
#include "servidor.h"

#define LISTA_VACIA 1
#define LISTA_LLEVA 2
#define SIN_MEM 3
#define CLA_DUP 4

typedef struct sNodo
{
    void *info;
    unsigned tamInfo;
    struct sNodo *sig;
}tNodo;

typedef tNodo *tLista;

typedef void(*Imp)(const void *info);
typedef int(*Cmp)(const void *e1, const void *e2);

void crearLista(tLista *p);
int ponerEnListaUltimo(tLista *pl, void *info, unsigned tam);
void mostrarLista(tLista *pl, Imp imprimir);
int ponerEnListaUltimoOrd(tLista *pl, void *info, unsigned tam, Cmp cmp);
int ponerEnListaUltimoOrdSinDup(tLista *pl, void *info, unsigned tam, Cmp cmp);
int sacarDeListaUltimo(tLista *pl, void *info, unsigned tam);
void ordenarLista(tLista *pl, unsigned tam, Cmp cmp);
void mostrarListaRecursiva(tLista *pl, Imp imprimir);

int ponerEnOrden(tLista *p, const void *d, unsigned cantBytes, Cmp cmp,
                 int (*acumular)(void **, unsigned *, const void *, unsigned));

tNodo **buscarMenorLista(tLista *pl, Cmp cmp);

#endif // LISTA_H_INCLUDED

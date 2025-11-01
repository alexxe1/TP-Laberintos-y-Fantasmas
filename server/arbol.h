#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX(A,B) ((A)>(B)?(A):(B))
#define MIN(A,B) ((A)<(B)?(A):(B))

#define EXITO 1
#define FALLO_MALLOC 2
#define DUPLICADO 3
#define ARBOL_VACIO 4
#define NO_EXISTE 5
#define TODO_OK 0

typedef struct sNodoArbol
{
 void * info;
 unsigned tamEl;
 struct sNodoArbol * izq;
 struct sNodoArbol * der;
}tNodoArbol;

typedef tNodoArbol* tArbol;

typedef struct
{
    char clave[11];
    unsigned int nroReg;
}
tIdx;

typedef int(*tCmp) (const void*, const void*);
typedef void(*tPrnt) (const void*);
typedef int(*COND)(const void *);
typedef int(*tCrIdx)(tArbol *a, void *reg, unsigned tamIdx, int nroReg, tCmp cmp);

void crearArbol (tArbol * a);
unsigned short insertarEnArbolR (tArbol * a, const void * elemento, unsigned esize, tCmp cmp);
unsigned short insertarEnArbolI (tArbol * a, const void * elemento, unsigned esize, tCmp cmp);

//FUNCIONES DE MOSTRAR
void mostrarEnOrden (tArbol * a, tPrnt mostrar);
void mostrarEnPreOrden (tArbol * a, tPrnt mostrar);
void mostrarEnPostOrden (tArbol * a, tPrnt mostrar);
void verHastaNivel (tArbol * ar, int nivel, tPrnt mostrar);
void verNivelNodos (tArbol * ar, int nivel, tPrnt mostrar);
void verDesdeNivelNodos (tArbol * ar, int nivel, tPrnt mostrar);

//FUNCIONES DE CONTAR
int contarHojas(tArbol * a);
int contarNoHojas(tArbol * a);
int contarNodos(tArbol * a);
int cantNodosSubArbolDerCond(tArbol *a, COND condicion);
int cantNodosSubArbolDerCondRec(tArbol *a, COND condicion, int cant);
int cantNodosCondClave(tArbol *a, COND condicion);
unsigned cantNodoArbolBin(const tArbol *a);
unsigned cantNodoHastaNivelArbolBin(const tArbol *a, int n);


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned alturaArbolBin(const tArbol *a);
int esArbolCompleto(tArbol *a);
int esArbolCompletoHastaNivel(tArbol *a, int nivel);
int esBalanceado(tArbol *a);
int esArbolCompleto2HastaNivel(tArbol * a, int nivel);
int esAVL(tArbol *a);

//FUNCIONES DE BUSQUEDA
tNodoArbol **buscarMenor(tArbol *a);
tNodoArbol **buscarMayor(tArbol *a);
tNodoArbol **buscarNodoArbol (tArbol * a,void * elemento, unsigned esize, tCmp cmp);
void *claveMayorMia(tArbol *a, tCmp cmp);
void *claveMenorMia(tArbol *a, tCmp cmp);
const tArbol *menorNodoNoClaveArbolBinBusq(const tArbol *p, const tArbol *menor, tCmp cmp);
int menorElemNoClaveArbolBinBusq(const tArbol *p, void *d, unsigned tam,tCmp cmp);

//FUNCIONES DE ELIMINAR
int eliminarNodo(tArbol *ar, void *clave, unsigned tamElem, tCmp cmp);
int eliminarRaizNodoArbolBinBusq(tArbol *ar);
void vaciarArbol(tArbol *a);


int crearArchIdx(tArbol *a, const char *nombreArch,const char *nombreArchIdx, unsigned tamInfo, unsigned tamIdx, tCrIdx crearIdx, tCmp cmp);
void cargarArchDesdeArbolBin(tArbol *a, FILE *archIdx, unsigned tamElem);
int cargarArbolBalanceado(tArbol *a,unsigned tamIdx, FILE *archBin, int li, int ls, tCmp cmp);
int cargarDesdeArchOrdenadoArbol(tArbol *a,unsigned tamIdx, const char *nombreArch, tCmp cmp);


#endif // ARBOL_H_INCLUDED

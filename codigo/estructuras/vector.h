#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <stdlib.h>
#include <string.h>
#include "../retornos.h"

#define TAM_BASE 8

typedef struct
{
    void* datos;
    size_t ce;
    size_t tamElemento;
    size_t tamMaximo;
} tVector;

int crearVector(tVector* vec, size_t tamElemento);
void destruirVector(tVector* vec);
int redimensionarVector(tVector* vec, size_t nuevoTam);

size_t obtenerLongitudVector(tVector* vec);
void* obtenerElementoVector(tVector* vec, size_t indice);

int insertarVector(tVector* vec, void* elemento);
void recorrerVector(tVector* vec, void (*funcion)(void* elemento));

#endif // VECTOR_H_INCLUDED

#include "vector.h"

int crearVector(tVector* vec, size_t tamElemento)
{
    vec->datos = malloc(TAM_BASE * tamElemento);

    if (vec->datos == NULL)
    {
        return 0;
    }

    vec->ce = 0;
    vec->tamElemento = tamElemento;
    vec->tamMaximo = TAM_BASE;

    return 1;
}

void destruirVector(tVector* vec)
{
    free(vec->datos);

    vec->tamElemento = 0;
    vec->tamMaximo = 0;
    vec->ce = 0;
}

int redimensionarVector(tVector* vec, size_t nuevaCantElem)
{
    void* aux = realloc(vec->datos, nuevaCantElem * vec->tamElemento);

    if (aux == NULL)
    {
        return 0;
    }

    vec->datos = aux;
    vec->tamMaximo = nuevaCantElem;

    return 1;
}

size_t obtenerLongitudVector(tVector* vec)
{
    return vec->ce;
}

void* obtenerElementoVector(tVector* vec, size_t indice)
{
    if (indice >= vec->ce)
    {
        return NULL;
    }

    return (vec->datos + (indice * vec->tamElemento));
}

int insertarVector(tVector* vec, void* elemento)
{
    // Verificamos que hay espacio primero
    if (vec->ce == vec->tamMaximo)
    {
        if (!redimensionarVector(vec, vec->ce + TAM_BASE))
        {
            return 0;
        }
    }

    memcpy((char*)vec->datos + (vec->ce * vec->tamElemento), elemento, vec->tamElemento);

    vec->ce++;

    return 1;
}

void recorrerVector(tVector* vec, void (*funcion)(void* elemento))
{
    void* elementoActual = vec->datos;
    size_t i;

    for (i = 0; i < vec->ce; i++)
    {
        funcion(elementoActual);
        elementoActual = (char*)elementoActual + vec->tamElemento;
    }
}

#include "lista.h"

void crearLista(tLista* lista)
{
    *lista = NULL;
}

int listaVacia(const tLista* lista)
{
    return *lista == NULL;
}

int listaLlena(const tLista* lista, size_t tam)
{
    tNodo* aux = (tNodo*)malloc(sizeof(tNodo));

    if (aux == NULL)
    {
        return 0;
    }

    aux->info = malloc(tam);

    if (aux->info == NULL)
    {
        free(aux);
        return 0;
    }

    free(aux->info);
    free(aux);

    return 1;
}

void vaciarLista(tLista* lista)
{
    tNodo* aux;

    while (*lista)
    {
        aux = *lista;

        *lista = aux->sig;

        free(aux->info);
        free(aux);
    }
}

int ponerAlFinal(tLista* lista, const void* elemento, size_t tam)
{
    tNodo* aux;

    while (*lista)
    {
        lista = &(*lista)->sig;
    }

    aux = (tNodo*)malloc(sizeof(tNodo));

    if (aux == NULL)
    {
        return 0;
    }

    aux->info = malloc(tam);

    if (aux->info == NULL)
    {
        free(aux);
        return 0;
    }

    memcpy(aux->info, elemento, tam);

    aux->tamInfo = tam;
    aux->sig = NULL;

    *lista = aux;

    return 1;
}

int sacarPrimeroLista(tLista* lista, void* elemento, size_t tam)
{
    tNodo* aux = *lista;

    if (aux == NULL)
    {
        return 0;
    }

    *lista = aux->sig;

    memcpy(elemento, aux->info, MIN(tam, aux->tamInfo));

    free(aux->info);
    free(aux);

    return 1;
}

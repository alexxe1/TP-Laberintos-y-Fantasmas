#include "cola.h"
#define minimo( X , Y )     ( ( X ) <= ( Y ) ? ( X ) : ( Y ) )

void crearCola(tCola *p)
{
    p->pri = NULL;
    p->ult = NULL;
}

int  colaLlena(const tCola *p, unsigned cantBytes)
{
    tNodoCola  *aux = (tNodoCola *)malloc(sizeof(tNodoCola));
    void   *info = malloc(cantBytes);
    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

int  ponerEnCola(tCola *p, const void *d, unsigned cantBytes)
{
    tNodoCola *nue = (tNodoCola *) malloc(sizeof(tNodoCola));

    if(nue == NULL || (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    if(p->ult)
        p->ult->sig = nue;
    else
        p->pri = nue;
    p->ult = nue;
    return 1;
}

int  verPrimeroCola(const tCola *p, void *d, unsigned cantBytes)
{
    if(p->pri == NULL)
        return 0;
    memcpy(d, p->pri->info, minimo(cantBytes, p->pri->tamInfo));
    return 1;
}

int  colaVacia(const tCola *p)
{
    return p->pri == NULL;
}

int sacarDeCola(tCola* cola, void* elemento, unsigned int tam)
{
    tNodoCola* aux = cola->pri;

    if (aux == NULL)
    {
        return 0;
    }

    cola->pri = aux->sig;

    memcpy(elemento, aux->info, minimo(aux->tamInfo, tam));

    free(aux->info);
    free(aux);

    if (cola->pri == NULL)
    {
        cola->ult = NULL;
    }

    return 1;
}

void vaciarCola(tCola *p)
{
    while(p->pri)
    {
        tNodoCola *aux = p->pri;
        p->pri = aux->sig;
        free(aux->info);
        free(aux);
    }
    p->ult = NULL;
}


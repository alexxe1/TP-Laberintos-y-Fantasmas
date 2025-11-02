#include "lista.h"
#include "servidor.h"

void crearLista(tLista *pl)
{
    *pl = NULL;
}

int ponerEnListaUltimo(tLista *pl, void *info, unsigned tam)
{
    tNodo *nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo)
    {
        return 1;
    }
    nuevo->info = malloc(tam);

    if(!nuevo->info)
    {
        free(nuevo);
        return 1;
    }

    memcpy(nuevo->info, info, tam);
    nuevo->tamInfo = tam;
    nuevo->sig = NULL;

    while(*pl)
    {
        pl = &(*pl)->sig;
    }

    *pl = nuevo;

    return 0;
}

void ordenarLista(tLista *pl, unsigned tam, Cmp cmp)
{
    tNodo **menor;
    tNodo *desenganchado;

    while(*pl)
    {
        menor = buscarMenorLista(pl,cmp);
        if(pl == menor)
        {
            pl = &(*pl)->sig;
        }
        else
        {
            desenganchado = *menor;
            *menor = desenganchado->sig;

            desenganchado->sig = *pl;
            *pl = desenganchado;
            pl = &(*pl)->sig;
        }

    }

}

tNodo **buscarMenorLista(tLista *pl, Cmp cmp)
{
    tNodo **min = pl;

    if(!min)
    {
        return NULL;
    }

    pl = &(*pl)->sig;

    while(*pl)
    {
        if(cmp((*pl)->info,(*min)->info) < 0)
        {
            min = pl;
        }
        pl = &(*pl)->sig;
    }
    return min;
}

int ponerEnListaUltimoOrdSinDup(tLista *pl, void *info, unsigned tam, Cmp cmp)
{
    tNodo *nuevo = (tNodo*)malloc(sizeof(tNodo));
    if(!nuevo)
    {
        return 1;
    }
    nuevo->info = malloc(tam);

    if(!nuevo->info)
    {
        free(nuevo);
        return 1;
    }

    memcpy(nuevo->info, info, tam);
    nuevo->tamInfo = tam;
    nuevo->sig = NULL;

    while(*pl && cmp(nuevo->info,(*pl)->info) > 0)
    {
        pl = &(*pl)->sig;
    }

    if(*pl && cmp(nuevo->info,(*pl)->info) == 0)
    {
        return 1;
    }

    if(*pl)
    {
        nuevo->sig = *pl;
    }
    *pl = nuevo;

    return 0;
}

int sacarDeListaUltimo(tLista *pl, void *info, unsigned tam)
{
    if(!*pl)
    {
        return 1;//Lista vacia;
    }
    while((*pl)->sig)
    {
        pl = &(*pl)->sig;
    }
    memcpy(info,(*pl)->info,MIN((*pl)->tamInfo,tam));
    free((*pl)->info);
    free(*pl);
    *pl = NULL;

    return 0;
}

void imprimirInt(const void *info)
{
    int *a = (int *)info;
    printf("%d->",*a);

}
void mostrarLista(tLista *pl, Imp imprimir)
{
    while(*pl)
    {
        imprimir((*pl)->info);

        pl = &(*pl)->sig;
    }
    printf("NULL");
}

void mostrarListaRecursiva(tLista *pl, Imp imprimir)
{
    if(*pl)
    {
        mostrarListaRecursiva(&(*pl)->sig, imprimir);
        imprimir((*pl)->info);
    }

}
void vaciarLista(tLista *pl)
{
    while(*pl)
    {
        pl = &(*pl)->sig;
    }
}

int verPriLista(tLista *pl, void *info, unsigned tamInfo)
{
    if(!*pl)
    {
        return LISTA_VACIA;
    }

    memcpy(info,(*pl)->info, MIN((*pl)->tamInfo, tamInfo));

    return TODO_OK;
}

int verUltimoLista(tLista *pl, void *info, unsigned tamInfo)
{
    if(!*pl)
    {
        return LISTA_VACIA;
    }

    while((*pl)->sig)
    {
        pl = &(*pl)->sig;
    }

    memcpy(info, (*pl)->info, MIN(tamInfo, (*pl)->tamInfo));

    return TODO_OK;
}

int ponerEnOrden(tLista *p, const void *d, unsigned cantBytes, Cmp cmp,
                 int (*acumular)(void **, unsigned *, const void *, unsigned))
{
    tNodo *nue;

    while (*p && cmp((*p)->info, d) < 0)   // <-- (1)
        p = &(*p)->sig;
    if (*p && cmp((*p)->info, d) == 0)     // <-- (2)
    {
        if (acumular)                           // <-- (3)
            if (!acumular(&(*p)->info, &(*p)->tamInfo, d, cantBytes))  // <-- (4)
                return SIN_MEM;                 // <-- (5)
        return CLA_DUP;                         // <-- (6)
    }

    if ((nue = (tNodo *)malloc(sizeof(tNodo))) == NULL ||   // <-- desde acá, ya es “terreno conocido”
        (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = *p;
    *p = nue;
    return TODO_OK;
}



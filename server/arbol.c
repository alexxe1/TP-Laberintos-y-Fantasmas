#include "arbol.h"

void crearArbol (tArbol * a)
{
    *a = NULL;
}

/////////////////////////////////////////////////////////////////////////////

unsigned short insertarEnArbolR (tArbol * a, const void * elemento, unsigned esize, tCmp cmp)
{

    int resultado;

    if (*a)
    {
        resultado = cmp(elemento, (*a)->info);
        if (resultado < 0)
            return insertarEnArbolR(&(*a)->izq, elemento, esize, cmp);
        else if (resultado > 0)
            return insertarEnArbolR(&(*a)->der, elemento, esize, cmp);
        else
            return DUPLICADO;
    }

    *a = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if (!(*a))
        return FALLO_MALLOC;

    (*a)->info = malloc(esize);
    if (!((*a)->info))
    {
        free(*a);
        return FALLO_MALLOC;
    }

    memcpy((*a)->info, elemento, esize);
    (*a)->tamEl = esize;
    (*a)->izq = NULL;
    (*a)->der = NULL;

    return EXITO;

}

/////////////////////////////////////////////////////////////////////////////

unsigned short insertarEnArbolI (tArbol * a, const void * elemento, unsigned esize, tCmp cmp)
{
    int resultado;

    while (*a)
    {
        resultado = cmp(elemento, (*a)->info);
        if (resultado < 0)
            a = &(*a)->izq;
        else if (resultado > 0)
            a = &(*a)->der;
        else
            return DUPLICADO;
    }

    *a = (tNodoArbol*)malloc(sizeof(tNodoArbol));
    if (!(*a))
        return FALLO_MALLOC;

    (*a)->info = malloc(esize);
    if (!((*a)->info))
    {
        free(*a);
        return FALLO_MALLOC;
    }

    memcpy((*a)->info, elemento, esize);
    (*a)->tamEl = esize;
    (*a)->izq = NULL;
    (*a)->der = NULL;

    return EXITO;
}

/////////////////////////////////////////////////////////////////////////////

void mostrarEnOrden (tArbol * a, tPrnt mostrar)
{
    if (*a)
    {
        mostrarEnOrden(&(*a)->izq, mostrar);
        mostrar((*a)->info);
        mostrarEnOrden(&(*a)->der, mostrar);
    }

}

/////////////////////////////////////////////////////////////////////////////

void mostrarEnPreOrden (tArbol * a, tPrnt mostrar)
{
    if (*a)
    {
        mostrar((*a)->info);
        mostrarEnPreOrden(&(*a)->izq, mostrar);
        mostrarEnPreOrden(&(*a)->der, mostrar);
    }

}

/////////////////////////////////////////////////////////////////////////////

void mostrarEnPostOrden (tArbol * a, tPrnt mostrar)
{
    if (*a)
    {
        mostrarEnPostOrden(&(*a)->izq, mostrar);
        mostrarEnPostOrden(&(*a)->der, mostrar);
        mostrar((*a)->info);
    }

}

/////////////////////////////////////////////////////////////////////////////
int contarHojas(tArbol * a)
{
    if(!*a)
    {
        return 0;
    }

    if(!(*a)->der && !(*a)->izq)
    {
        return contarHojas(&(*a)->izq) + contarHojas(&(*a)->der) + 1;
    }
    return contarHojas(&(*a)->izq) + contarHojas(&(*a)->der);
}
/////////////////////////////////////////////////////////////////////////////
int contarNoHojas(tArbol * a)
{
    if (!*a)
    {
        return 0;
    }

    if((*a)->der || (*a)->izq)
    {
        return contarNoHojas(&(*a)->izq) + contarNoHojas(&(*a)->der) + 1;
    }
    return contarNoHojas(&(*a)->izq) + contarNoHojas(&(*a)->der);

}
/////////////////////////////////////////////////////////////////////////////

int contarNodos(tArbol * a)
{
    if(!*a)
    {
        return 0;
    }

    return contarNodos(&(*a)->izq) + contarNodos(&(*a)->der) + 1;
}
/////////////////////////////////////////////////////////////////////////////

void *claveMayorMia(tArbol *a, tCmp cmp)
{
    void *mayor, *mayorIzq, *mayorDer;

    if(*a)
    {

        mayorIzq = claveMayorMia(&(*a)->izq, cmp);
        if(!(*a)->izq)
        {
            mayorIzq = (*a)->info;
        }

        if(mayorIzq && cmp((*a)->info, mayorIzq) > 0)
        {
            mayorIzq = (*a)->info;
        }

        mayorDer = claveMayorMia(&(*a)->der, cmp);

        if(!(*a)->der)
        {
            mayorDer = (*a)->info;
        }

        if(mayorDer && cmp((*a)->info,mayorDer) > 0)
        {
            mayorDer = (*a)->info;
        }

        if(cmp(mayorDer, mayorIzq) > 0)
        {
            mayor = mayorDer;
        }
        else
        {
            mayor = mayorIzq;
        }

        return mayor;
    }

    return NULL;
}
/////////////////////////////////////////////////////////////////////////////

void *claveMenorMia(tArbol *a, tCmp cmp)
{
    if (*a == NULL)
        return NULL;

    void *menor = (*a)->info;
    void *menorIzq = claveMenorMia(&(*a)->izq, cmp);
    void *menorDer = claveMenorMia(&(*a)->der, cmp);

    if (menorIzq && cmp(menorIzq, menor) < 0)
        menor = menorIzq;

    if (menorDer && cmp(menorDer, menor) < 0)
        menor = menorDer;

    return menor;
}
/////////////////////////////////////////////////////////////////////////////

int cantNodosSubArbolDerCond(tArbol *a, COND condicion)
{
    return cantNodosSubArbolDerCondRec(&(*a)->der, condicion, 0);
}
/////////////////////////////////////////////////////////////////////////////

int cantNodosSubArbolDerCondRec(tArbol *a, COND condicion, int cant)
{
    if(*a)
    {
        cant = cantNodosSubArbolDerCondRec(&(*a)->izq, condicion, cant);
        if(condicion((*a)->info))
        {
            cant++;
        }
        cant = cantNodosSubArbolDerCondRec(&(*a)->der, condicion, cant);

        return cant;
    }
    return cant;
}
/////////////////////////////////////////////////////////////////////////////

tNodoArbol **buscarNodoArbol (tArbol * a,void * elemento, unsigned esize, tCmp cmp)
{

 int resultado;

 if (*a && (resultado = cmp(elemento, (*a)->info)))
 {
  if (resultado < 0)
     return buscarNodoArbol(&(*a)->izq, elemento, esize, cmp);
   else if (resultado > 0)
    return buscarNodoArbol(&(*a)->der, elemento, esize, cmp);

 }
 if(!*a)
 {
     return NULL;
 }


 memcpy(elemento, (*a)->info, MIN((*a)->tamEl,esize));

 return (tNodoArbol**)a;
}

tNodoArbol **buscarNodoNoClave(tArbol *a, void *info, unsigned tamInfo, tCmp cmp)
{
    tNodoArbol **resultado = NULL;

    if(!*a)
    {
        return NULL;
    }

    if(!cmp((*a)->info,info))
    {
        memcpy(info, (*a)->info, tamInfo);
        return (tNodoArbol**)a;
    }

    resultado = buscarNodoNoClave(&(*a)->izq, info, tamInfo, cmp);

    return resultado ? resultado : buscarNodoNoClave(&(*a)->der,info,tamInfo,cmp);
}

int menorElemNoClaveArbolBinBusq(const tArbol *p, void *d, unsigned tam,tCmp cmp)
{
    const tArbol *menor = p;
    if (!*p)
        return 0;

    menor = menorNodoNoClaveArbolBinBusq(&(*p)->izq, menor, cmp);
    menor = menorNodoNoClaveArbolBinBusq(&(*p)->der, menor, cmp);

    memcpy(d, (*menor)->info, MIN(tam, (*menor)->tamEl));

    return 1;
}

const tArbol *menorNodoNoClaveArbolBinBusq(const tArbol *p,
                                                  const tArbol *menor,
                                                  tCmp cmp)
{
    if (!*p)
        return menor;

    if (cmp((*p)->info, (*menor)->info) < 0)
        menor = p;

    menor = menorNodoNoClaveArbolBinBusq(&(*p)->izq, menor, cmp);
    menor = menorNodoNoClaveArbolBinBusq(&(*p)->der, menor, cmp);

    return menor;
}

/////////////////////////////////////////////////////////////////////////////
int cantNodosCondClave(tArbol *a, COND condicion)
{
    if(!*a)
    {
        return 0;
    }

    if(condicion((*a)->info))
    {
        return cantNodosCondClave(&(*a)->izq, condicion) + cantNodosCondClave(&(*a)->der,condicion)+1;
    }else
    {
        return cantNodosCondClave(&(*a)->izq, condicion) + cantNodosCondClave(&(*a)->der,condicion);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
tNodoArbol **buscarMenor(tArbol *a)
{
    if(!(*a))
    {
        return NULL;
    }
    while((*a)->izq)
    {
        a = &(*a)->izq;
    }

    return (tNodoArbol**)a;
}

tNodoArbol **buscarMayor(tArbol *a)
{
    if(!(*a))
    {
        return NULL;
    }
    while((*a)->der)
    {
        a = &(*a)->der;
    }

    return (tNodoArbol**)a;
}

unsigned alturaArbolBin(const tArbol *a)
{
    unsigned hi, hd;
    if(!*a)
    {
        return 0;
    }
    hi = alturaArbolBin(&(*a)->izq);
    hd = alturaArbolBin(&(*a)->der);

    return hi>hd ? hi:hd + 1;
}

unsigned cantNodoArbolBin(const tArbol *a)
{
    if(!*a)
    {
        return 0;
    }

    return cantNodoArbolBin(&(*a)->izq) + cantNodoArbolBin(&(*a)->der) + 1;
}

unsigned cantNodoHastaNivelArbolBin(const tArbol *a, int n)
{
    if(!*a)
    {
        return 0;
    }

    if(n == 0)
    {
        return 1;
    }

    return cantNodoHastaNivelArbolBin(&(*a)->izq, n - 1) + cantNodoHastaNivelArbolBin(&(*a)->izq, n - 1) + 1;
}

/////////////////////////////////////////////////////////////////////////////

void verHastaNivel (tArbol * ar, int nivel, tPrnt mostrar)
{
 if (!*ar || nivel < 0)
     return;

 mostrar((*ar)->info);
 verHastaNivel(&(*ar)->izq, nivel-1, mostrar);
 verHastaNivel(&(*ar)->der, nivel-1, mostrar);

}

void verNivelNodos (tArbol * ar, int nivel, tPrnt mostrar)
{
    if(!*ar)
    {
        return;
    }

    if(nivel == 0)
    {
        mostrar((*ar)->info);
        return;
    }

    verNivelNodos(&(*ar)->izq, nivel-1, mostrar);
    verNivelNodos(&(*ar)->der, nivel -1, mostrar);

    return;
}

void verDesdeNivelNodos (tArbol * ar, int nivel, tPrnt mostrar)
{
    if(!*ar)
    {
        return;
    }

    if(nivel <= 0)
    {
        mostrar((*ar)->info);
    }

    verDesdeNivelNodos(&(*ar)->izq, nivel - 1, mostrar);
    verDesdeNivelNodos(&(*ar)->der, nivel - 1, mostrar);

    return;
}

int eliminarNodo(tArbol *ar, void *clave, unsigned tamElem, tCmp cmp)
{
    tNodoArbol **elim = buscarNodoArbol(ar,clave, tamElem, cmp);

    if(!*elim)
    {
        return NO_EXISTE;
    }

    if(!(*ar)->izq && !(*ar)->der)
    {
        free((*ar)->info);
        free(*ar);
        *ar = NULL;
    }else
    {
        eliminarRaizNodoArbolBinBusq(elim);
    }

    return EXITO;
}

int eliminarRaizNodoArbolBinBusq(tArbol *ar)
{
    tNodoArbol **remp, *elim;

    if(!(*ar))
    {
        return ARBOL_VACIO;
    }
    remp = alturaArbolBin(&(*ar)->izq) > alturaArbolBin(&(*ar)->der) ?
           buscarMayor(&(*ar)->izq) : buscarMenor(&(*ar)->der);

    elim = *remp;
    (*ar)->info = elim->info;
    (*ar)->tamEl = elim->tamEl;

    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);

    return EXITO;

}

int esArbolCompleto(tArbol *a)
{
    int h = alturaArbolBin(a);
    return cantNodoArbolBin(a) == (pow(2,h) -1);
}

int esArbolCompletoHastaNivel(tArbol *a, int nivel)
{
    int h = alturaArbolBin(a);

    if(nivel > h - 1)
    {
        nivel = h - 1;
    }

    return cantNodoHastaNivelArbolBin(a,nivel) == (pow(2,nivel + 1) - 1);
}

int esArbolCompleto2HastaNivel(tArbol * a, int nivel)
{
    if(!*a)
    {
        return nivel < 0;
    }

    if(nivel == 0)
    {
        return 1;
    }

    return esArbolCompleto2HastaNivel(&(*a)->izq, nivel - 1) && esArbolCompleto2HastaNivel(&(*a)->izq, nivel - 1);

}
int esBalanceado(tArbol *a)
{
    int n = alturaArbolBin(a) - 2;

    return esArbolCompleto2HastaNivel(a,n);
}

int esAVL(tArbol *a)
{
    unsigned hi,hd;

    if(!*a)
    {
        return 1;
    }

    hi = alturaArbolBin(&(*a)->izq);
    hd = alturaArbolBin(&(*a)->der);

    if(abs(hi - hd) > 1)
    {
        return 0;
    }

    return esAVL(&(*a)->izq) && esAVL(&(*a)->der);
}

/////////////////////////////////////////////////////////////////////////////////////////////

int crearArchIdx(tArbol *a, const char *nombreArch,const char *nombreArchIdx, unsigned tamInfo, unsigned tamClave, tCrIdx crearIdx, tCmp cmp)
{
    int nroReg = 0;
    void *reg;
    FILE *archBin = fopen(nombreArch, "rb");
    if(!archBin)
    {
        return 1;
    }

    FILE *archIdx = fopen(nombreArchIdx, "wb");
    if(!archIdx)
    {
        fclose(archBin);
        return 1;
    }

    reg = malloc(tamInfo);
    if(!reg)
    {
        fclose(archBin);
        fclose(archIdx);
        return FALLO_MALLOC;
    }

    while(fread(reg, tamInfo, 1, archBin))
    {
        crearIdx(a, reg,tamClave, nroReg++, cmp);
    }

    cargarArchDesdeArbolBin(a, archIdx, sizeof(tIdx));

    free(reg);
    fclose(archBin);
    fclose(archIdx);

    return TODO_OK;
}

void cargarArchDesdeArbolBin(tArbol *a, FILE *archIdx, unsigned tamElem)
{

    if(!*a || !archIdx)
    {
        return;
    }

    cargarArchDesdeArbolBin(&(*a)->izq, archIdx, tamElem);
    fwrite((*a)->info, tamElem, 1, archIdx);
    cargarArchDesdeArbolBin(&(*a)->der, archIdx, tamElem);
}

int cargarDesdeArchOrdenadoArbol(tArbol *a,unsigned tamIdx, const char *nombreArch, tCmp cmp)
{
    int cantReg;

    FILE *archBin = fopen(nombreArch, "rb");
    if(!archBin)
    {
        return 1;
    }

    fseek(archBin,0,SEEK_END);
    cantReg = ftell(archBin)/tamIdx;
    fseek(archBin,0,SEEK_SET);

    cargarArbolBalanceado(a,tamIdx, archBin, 0, cantReg - 1,cmp);

    return TODO_OK;
}

int cargarArbolBalanceado(tArbol *a,unsigned tamIdx, FILE *archBin, int li, int ls, tCmp cmp)
{
    void *reg;
    int mid;

    if(li > ls)
    {
        return 0;
    }
    reg = malloc(tamIdx);
    if(!reg)
    {
        return FALLO_MALLOC;
    }

    mid = (li + ls)/2;

    fseek(archBin, mid*tamIdx, SEEK_SET);
    fread(reg,tamIdx,1,archBin);

    insertarEnArbolR(a,reg, tamIdx,cmp);

    free(reg);

    cargarArbolBalanceado(a,tamIdx, archBin, li, mid - 1, cmp);
    cargarArbolBalanceado(a,tamIdx, archBin, mid + 1, ls,cmp);

    return TODO_OK;
}
void vaciarArbol(tArbol *a)
{
    if (!*a)
        return;

    vaciarArbol(&(*a)->izq);
    vaciarArbol(&(*a)->der);
    free((*a)->info);        // libero tIdx
    free(*a);                // libero el nodo

    *a = NULL;
}

#include "cola.h"

void crearCola(tCola *p)
{
    p->pri = 0;
    p->ult = 0;
    p->tDisp = TAM_COLA;

}

void vaciarCola(tCola *p)
{
    p->pri = 0;
    p->ult = 0;
    p->tDisp = TAM_COLA;
}

int colaLlena(tCola *p, unsigned cantB)
{
    return p->tDisp < (cantB + sizeof(unsigned));
}

int colaVacia(const tCola *p)
{
    return p->tDisp == TAM_COLA;
}

int ponerEncola(tCola *p, const void *info, unsigned cantB)
{
    unsigned ini, fin;
    if(p->tDisp < (cantB + sizeof(unsigned)))
       {
           return 0;
       }
    p->tDisp -= cantB + sizeof(unsigned);

    ini =  MIN(sizeof(unsigned), TAM_COLA - p->ult);
    fin = sizeof(unsigned) - ini;

    if(ini > 0)
    {
        memcpy(p->cola + p->ult,&cantB, ini);
    }

    if(fin > 0)
    {
        memcpy(p->cola, ((char*)&cantB), fin);
        p->ult = fin;
    }
    else
    {
        p->ult += ini;
    }

    ini =  MIN(cantB, TAM_COLA - p->ult);
    fin = cantB - ini;

    if(ini > 0)
    {
        memcpy(p->cola + p->ult,info, ini);
    }

    if(fin > 0)
    {
        memcpy(p->cola, ((char*)info) + ini, fin);
        p->ult = fin;
    }
    else
    {
        p->ult += ini;
    }

    return 1;
}

int sacarDeCola(tCola *p, void *info, unsigned cantB)
{
    unsigned ini, fin, tamEnCola, tamMin;

    if(p->tDisp == TAM_COLA)
    {
        return 0;
    }

    ini =  MIN(sizeof(unsigned), TAM_COLA - p->pri);
    fin = sizeof(unsigned) - ini;

    if(ini > 0)
    {
        memcpy(&tamEnCola, p->cola + p->pri, ini);
    }
    if(fin > 0)
    {
        memcpy(((char*)&tamEnCola) + ini, p->cola, fin);
        p->pri = fin;
    }
    else
    {
        p->pri += ini;
    }
    p->tDisp += sizeof(unsigned) + tamEnCola;

    tamMin = MIN(tamEnCola, cantB);

    ini = MIN(tamMin, TAM_COLA - p->pri);
    fin = tamMin - ini;

    if(ini > 0)
    {
        memcpy(info, p->cola + p->pri, ini);
    }
    if(fin > 0)
    {
        memcpy(((char*)info) + ini, p->cola, fin);
        p->pri = fin;
    }
    else
    {
        p->pri += tamEnCola;
    }

   return 1;
}

int verPri(const tCola *p, void *info, unsigned tam)
{
    unsigned ini, fin, tamElem, tamMin;
    unsigned priAux = p->pri;

    if(p->tDisp == TAM_COLA)
    {
        return 0;
    }

    ini = MIN(sizeof(unsigned), TAM_COLA - p->pri);
    fin = sizeof(unsigned) - ini;

    if(ini > 0)
    {
        memcpy(&tamElem, p->cola + p->pri, ini);

    }
    if(fin > 0)
    {
        memcpy(((char*)&tamElem) + ini, p->cola, fin);
        priAux = fin;
    }
    else
    {
        priAux += ini;
    }

    tamMin = MIN(tamElem, tam);

    ini = MIN(tamMin, TAM_COLA - priAux);
    fin = tamMin - ini;

    if(ini > 0)
    {
        memcpy(info, p->cola + priAux, ini);
    }
    if(fin > 0)
    {
        memcpy(((char*)info) + ini, p->cola, fin);
    }

    return 1;
}



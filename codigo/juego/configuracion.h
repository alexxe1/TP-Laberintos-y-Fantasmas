#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../retornos.h"

#define CONFIG "config.txt"
#define TAM_LINEA 1024
#define SEPARADOR ':'

typedef struct
{
    size_t filas;
    size_t columnas;
    size_t vidasIniciales;
    size_t maxFantasmas;
    size_t maxPremios;
    size_t maxVidasExtras;
} tConfiguracion;

int cargarArchivoConfiguracion(tConfiguracion* configuracion);

#endif // CONFIGURACION_H_INCLUDED

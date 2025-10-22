#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../retornos.h"

#define CONFIG "config.txt"
#define TAM_LINEA 1024
#define SEPARADOR ':'

#define ERROR_ARCH 0
#define ERROR_FILAS -1
#define ERROR_COLUMNAS -2
#define ERROR_VIDAS_INI -3
#define ERROR_FANT -4
#define ERROR_PREMIOS -5
#define ERROR_VIDAS_EXT -6
#define MAX_FILAS 50
#define MAX_COLUM 50
#define MIN_FILAS 8
#define MIN_COLUM 8
#define MAX_VIDAS_INI 99

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

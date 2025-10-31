#ifndef CONFIGURACION_H_INCLUDED
#define CONFIGURACION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../retornos.h"

#define CONFIG "config.txt"
#define TAM_LINEA 1024
#define SEPARADOR ':'

#define ERROR_ARCHIVO 0
#define ERROR_FILAS -1
#define ERROR_COLUMNAS -2
#define ERROR_VIDAS_INICIALES -3
#define ERROR_FANTASMAS -4
#define ERROR_PREMIOS -5
#define ERROR_VIDAS_EXTRAS -6

#define MAX_FILAS 50
#define MAX_COLUMNAS 50
#define MIN_FILAS 8
#define MIN_COLUMNAS 8
#define MAX_VIDAS_INICIALES 99

#define DIV_MAX_FANTASMAS 25
#define DIV_MAX_PREMIOS 20
#define DIV_MAX_VIDAS_EXTRAS 25

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
int leerValor(FILE* archivo, size_t* destino, char separador, int codigoError);

#endif // CONFIGURACION_H_INCLUDED

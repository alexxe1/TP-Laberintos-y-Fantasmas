#include "configuracion.h"

int cargarArchivoConfiguracion(tConfiguracion* configuracion)
{
    FILE* pConfig = fopen(CONFIG, "rt");
    char linea[TAM_LINEA];
    char* separador;
    unsigned long tmp;
    int maxFantasmas, maxPremios, maxVidasExtra;

    if (!pConfig)
        return ERROR;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR_ARCH;
    }

    // Leer las filas
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->filas = (size_t)tmp;

    if(configuracion->filas > MAX_FILAS || configuracion->filas < MIN_FILAS)
    {
        fclose(pConfig);
        return ERROR_FILAS;
    }

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR_ARCH;
    }

    // Leer las columnas
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->columnas = (size_t)tmp;

    if(configuracion->columnas > MAX_COLUM ||configuracion->columnas < MIN_COLUM)
    {
        fclose(pConfig);
        return ERROR_COLUMNAS;
    }

    // Calculo maximo de entidades acorde al tamaño del laberinto
    maxFantasmas = (configuracion->filas * configuracion->columnas / 25);
    maxPremios = (configuracion->filas * configuracion->columnas / 20);
    maxVidasExtra = (configuracion->filas * configuracion->columnas / 25);

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR_ARCH;
    }

    // Leer las vidas iniciales
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->vidasIniciales = (size_t)tmp;

    if(configuracion->vidasIniciales <= 0 || configuracion->vidasIniciales > MAX_VIDAS_INI)
    {
        fclose(pConfig);
        return ERROR_VIDAS_INI;
    }

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR_ARCH;
    }

    // Leer los máximos fantasmas permitidos
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->maxFantasmas = (size_t)tmp;

    if(configuracion->maxFantasmas < 0 || configuracion->maxFantasmas > maxFantasmas)
    {
        fclose(pConfig);
        return ERROR_FANT;
    }

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR_ARCH;
    }

    // Leer los máximos premios permitidos
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->maxPremios = (size_t)tmp;

    if(configuracion->maxPremios < 0 || configuracion->maxPremios > maxPremios)
    {
        fclose(pConfig);
        return ERROR_PREMIOS;
    }

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR_ARCH;
    }

    // Leer las máximas vidas extras permitidas
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->maxVidasExtras = (size_t)tmp;

    if(configuracion->maxVidasExtras < 0 || configuracion->maxVidasExtras > maxVidasExtra)
    {
        fclose(pConfig);
        return ERROR_VIDAS_EXT;
    }

    fclose(pConfig);

    return EXITO;
}

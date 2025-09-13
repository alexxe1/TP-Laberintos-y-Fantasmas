#include "configuracion.h"

int cargarArchivoConfiguracion(tConfiguracion* configuracion)
{
    // Para hacer
    FILE* pConfig = fopen(CONFIG, "rt");
    char linea[TAM_LINEA];
    char* sep;
    unsigned long tmp;

    if(!pConfig)
        return ERROR;

    fgets(linea, sizeof(linea), pConfig);
    sep = strchr(linea, SEPARADOR);
    if(!sep)
        return ERROR;

    sscanf(sep + 1, "%lu", &tmp);
    configuracion->filas = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    sep = strchr(linea, SEPARADOR);
    if(!sep)
        return ERROR;

    sscanf(sep + 1, "%lu", &tmp);
    configuracion->columnas = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    sep = strchr(linea, SEPARADOR);
    if(!sep)
        return ERROR;

    sscanf(sep + 1, "%lu", &tmp);
    configuracion->vidasIniciales = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    sep = strchr(linea, SEPARADOR);
    if(!sep)
        return ERROR;

    sscanf(sep + 1, "%lu", &tmp);
    configuracion->maxFantasmas = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    sep = strchr(linea, SEPARADOR);
    if(!sep)
        return ERROR;

    sscanf(sep + 1, "%lu", &tmp);
    configuracion->maxPremios = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    sep = strchr(linea, SEPARADOR);
    if(!sep)
        return ERROR;

    sscanf(sep + 1, "%lu", &tmp);
    configuracion->maxVidasExtras = (size_t)tmp;

    fclose(pConfig);
    return EXITO;
}

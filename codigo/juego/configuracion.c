#include "configuracion.h"

int cargarArchivoConfiguracion(tConfiguracion* configuracion)
{
    FILE* pConfig = fopen(CONFIG, "rt");
    char linea[TAM_LINEA];
    char* separador;
    unsigned long tmp;

    if (!pConfig)
        return ERROR;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR;
    }

    // Leer las filas
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->filas = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR;
    }

    // Leer las columnas
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->columnas = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR;
    }

    // Leer las vidas iniciales
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->vidasIniciales = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR;
    }

    // Leer los máximos fantasmas permitidos
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->maxFantasmas = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR;
    }

    // Leer los máximos premios permitidos
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->maxPremios = (size_t)tmp;

    fgets(linea, sizeof(linea), pConfig);
    separador = strchr(linea, SEPARADOR);

    if(!separador)
    {
        fclose(pConfig);
        return ERROR;
    }

    // Leer las máximas vidas extras permitidas
    sscanf(separador + 1, "%lu", &tmp);
    configuracion->maxVidasExtras = (size_t)tmp;

    fclose(pConfig);

    return EXITO;
}

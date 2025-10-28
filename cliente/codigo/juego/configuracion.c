#include "configuracion.h"

int cargarArchivoConfiguracion(tConfiguracion* configuracion)
{
    FILE* pConfig = fopen(CONFIG, "rt");
    int maxFantasmas, maxPremios, maxVidasExtra;
    int resultado;

    if (!pConfig)
        return ERROR;

    // Leer las filas
    resultado = leerValor(pConfig, &configuracion->filas, SEPARADOR, ERROR_FILAS);

    if (resultado != EXITO)
    {
        fclose(pConfig);
        return resultado;
    }

    if(configuracion->filas < MIN_FILAS || configuracion->filas > MAX_FILAS)
    {
        fclose(pConfig);
        return ERROR_FILAS;
    }

    // Leer las columnas
    resultado = leerValor(pConfig, &configuracion->columnas, SEPARADOR, ERROR_COLUMNAS);

    if (resultado != EXITO)
    {
        fclose(pConfig);
        return resultado;
    }

    if (configuracion->columnas < MIN_COLUMNAS || configuracion->columnas > MAX_COLUMNAS)
    {
        fclose(pConfig);
        return ERROR_COLUMNAS;
    }

    // Calcular los límites máximos
    maxFantasmas = (configuracion->filas * configuracion->columnas / DIV_MAX_FANTASMAS);
    maxPremios = (configuracion->filas * configuracion->columnas / DIV_MAX_PREMIOS);
    maxVidasExtra = (configuracion->filas * configuracion->columnas / DIV_MAX_VIDAS_EXTRAS);

    // Leer las vidas iniciales
    resultado = leerValor(pConfig, &configuracion->vidasIniciales, SEPARADOR, ERROR_VIDAS_INICIALES);

    if (resultado != EXITO)
    {
        fclose(pConfig);
        return resultado;
    }

    if (configuracion->vidasIniciales == 0 || configuracion->vidasIniciales > MAX_VIDAS_INICIALES)
    {
        fclose(pConfig);
        return ERROR_VIDAS_INICIALES;
    }

    // Leer máxima cantidad de fantasmas
    resultado = leerValor(pConfig, &configuracion->maxFantasmas, SEPARADOR, ERROR_FANTASMAS);

    if (resultado != EXITO)
    {
        fclose(pConfig);
        return resultado;
    }

    if (configuracion->maxFantasmas > (size_t)maxFantasmas)
    {
        fclose(pConfig);
        return ERROR_FANTASMAS;
    }

    // Leer máximo de premios
    resultado = leerValor(pConfig, &configuracion->maxPremios, SEPARADOR, ERROR_PREMIOS);

    if (resultado != EXITO)
    {
        fclose(pConfig);
        return resultado;
    }

    if (configuracion->maxPremios > (size_t)maxPremios)
    {
        fclose(pConfig);
        return ERROR_PREMIOS;
    }

    // Leer máximo de vidas extras
    resultado = leerValor(pConfig, &configuracion->maxVidasExtras, SEPARADOR, ERROR_VIDAS_EXTRAS);

    if (resultado != EXITO)
    {
        fclose(pConfig);
        return resultado;
    }

    if (configuracion->maxVidasExtras > (size_t)maxVidasExtra)
    {
        fclose(pConfig);
        return ERROR_VIDAS_EXTRAS;
    }

    fclose(pConfig);

    return EXITO;
}

int leerValor(FILE* archivo, size_t* destino, char separador, int codigoError)
{
    char linea[TAM_LINEA];
    char* sep;
    unsigned long temp;

    if (fgets(linea, sizeof(linea), archivo) == NULL)
    {
        return ERROR_ARCHIVO;
    }

    // Eliminar salto de línea
    linea[strcspn(linea, "\r\n")] = '\0';

    sep = strchr(linea, separador);

    if (sep == NULL || sscanf(sep + 1, "%lu", &temp) != 1)
    {
        return ERROR_ARCHIVO;
    }

    *destino = (size_t)temp;

    return EXITO;
}

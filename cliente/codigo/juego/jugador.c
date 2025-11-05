#include "jugador.h"

void crearJugador(tJugador* jugador, const char * nombJug, tConfiguracion* configuracion, size_t fila, size_t columna)
{
    jugador->posInicial.fila = fila;
    jugador->posInicial.columna = columna;

    jugador->posActual.fila = fila;
    jugador->posActual.columna = columna;

    jugador->puntajeTotal = 0;
    jugador->vidas = configuracion->vidasIniciales > TOPE_VIDAS ? TOPE_VIDAS : configuracion->vidasIniciales;

    strcpy(jugador->nombre, nombJug);
    crearCola(&jugador->colaMovimientos);
}

void acomodarJugador (tJugador* jugador, size_t fila, size_t columna)
{
    jugador->posInicial.fila = fila;
    jugador->posInicial.columna = columna;

    jugador->posActual.fila = fila;
    jugador->posActual.columna = columna;
}

void destruirJugador(tJugador* jugador)
{
    vaciarCola(&jugador->colaMovimientos);
}

void dibujarJugador(tJugador* jugador, size_t fila, size_t columna)
{
    printf("%c", JUGADOR);
}

char moverJugador(tJugador* jugador, char direccion, const tLaberinto* laberinto)
{
    size_t nuevaFila = jugador->posActual.fila;
    size_t nuevaColumna = jugador->posActual.columna;

    switch (direccion)
    {
    case IZQUIERDA:
        if (nuevaColumna > 0)
            nuevaColumna--;
        break;

    case DERECHA:
        if (nuevaColumna < laberinto->columnas - 1)
            nuevaColumna++;
        break;

    case ARRIBA:
        if (nuevaFila > 0)
            nuevaFila--;
        break;

    case ABAJO:
        if (nuevaFila < laberinto->filas - 1)
            nuevaFila++;
        break;

    default: // No nos movemos si no hay dirección
        return FALSO;
    }

    // No permitir moverse si hay una pared
    if (laberinto->casillas[nuevaFila][nuevaColumna] == '#')
        return FALSO;

    // Si después de verificar todo, no hay problema, nos movemos
    jugador->posActual.fila = nuevaFila;
    jugador->posActual.columna = nuevaColumna;

    return VERDADERO;
}

tPosicion obtenerPosJugador(tJugador* jugador)
{
    tPosicion pos;

    pos.fila = jugador->posActual.fila;
    pos.columna = jugador->posActual.columna;

    return pos;
}

void ingresarNombre(char * cadena, unsigned longitud)
{
    size_t len;

    fflush(stdin);

    if (fgets(cadena, longitud, stdin))
    {
        len = strlen(cadena);

        if (len > 0 && cadena[len - 1] == '\n')
            cadena[len - 1] = '\0';
    }
}

char esNombreValido (const char * nombre)
{
    int longitud = 0;

    if (!nombre || *nombre == '\0')
        return ERROR;

    if (!esLetra(*nombre))
        return ERROR; // debe empezar con letra

    while (*nombre)
    {
        if (!esLetra(*nombre) && !esNumero(*nombre) && *nombre != '_')
            return ERROR;

        nombre++;
        longitud++;
    }

    if (longitud < 3)
        return ERROR;

    return EXITO;
}

char esLetra (int caracter)
{
    return ((caracter >= 'A' && caracter <= 'Z') || (caracter >= 'a' && caracter <= 'z') ? EXITO : ERROR);
}

char esNumero (int caracter)
{
    return ((caracter) >= '0' && caracter <= '9' ? EXITO : ERROR);
}

char esEspacio (int caracter)
{
    return (caracter == ' ' ? EXITO : ERROR);
}

void volverYDescontar(tJugador* jugador)
{
    jugador->posActual.fila = jugador->posInicial.fila;
    jugador->posActual.columna = jugador->posInicial.columna;
    jugador->vidas--;
}

char jugadorSinVidas(tJugador* jugador)
{
    return (jugador->vidas <= 0 ? VERDADERO : FALSO);
}

char jugadorEnSalida(tJugador* jugador, tLaberinto* laberinto)
{
    char casilla = obtenerCasillaLaberinto(laberinto, jugador->posActual. fila, jugador->posActual.columna) ;

    return casilla == SALIDA ? VERDADERO : FALSO;
}

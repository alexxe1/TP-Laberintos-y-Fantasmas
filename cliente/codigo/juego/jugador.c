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

void ingresarNombre (char * cadena, unsigned longitud)
{
 fflush(stdin);
 fgets(cadena, longitud, stdin);
 while (*cadena != '\n')
    cadena++;

 *cadena = '\0';
}

char esNombreValido (const char * nombre)
{
 if (!nombre || *nombre == '\0')
    return ERROR;

 while (*nombre)
 {
  if (!esLetra(*nombre) && !esNumero(*nombre) && !esEspacio(*nombre))
     return ERROR;

  nombre++;
 }

 return EXITO;
}

char esLetra (int car)
{
 return ( (car >= 'A' && car <= 'Z') || (car >= 'a' && car <= 'z') ? EXITO : ERROR);
}

char esNumero (int car)
{
 return ( (car) >= '0' && car <= '9' ? EXITO : ERROR);
}

char esEspacio (int car)
{
 return (car == ' ' ? EXITO : ERROR);
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



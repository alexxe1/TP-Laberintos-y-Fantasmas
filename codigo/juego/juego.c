#include "juego.h"
void imprimirInt(const void* c);
void imprimirPos(const void *p);

int empezarJuego()
{
    unsigned char juegoTerminado = FALSO;
    short int estado;
    tLaberinto laberinto;
    tEntidades entidades;
    tConfiguracion configuracion;

    // Cargamos el archivo de configuración
    if (!cargarArchivoConfiguracion(&configuracion))
        return ERROR;

    // Cargamos laberinto de un .txt
    if (!crearLaberintoArchivo(&laberinto))
      return ERROR;

    //Inicializo semilla random
    srand((unsigned)time(NULL));

    // Generamos un laberinto aleatorio
   // if(!crearLaberintoAleatorio(&laberinto, &configuracion))
     //   return ERROR;

    // Inicializar el vector de fantasmas
    if (!crearVector(&entidades.fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(&laberinto);
        return ERROR;
    }

    // Leemos el laberinto y lo interpretamos para generar todo
    if (!procesarEntidades(&laberinto, &entidades, &configuracion))
    {
        destruirLaberinto(&laberinto);
        destruirVector(&entidades.fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        return ERROR;
    }

    /// Bucle principal
    while (!juegoTerminado)
    {
        system("cls");

        estado = actualizarJuego(&laberinto, &entidades, &juegoTerminado);
        dibujarJuego(&laberinto, &entidades);

        Sleep(100);
    }


    registroMov(&entidades.jugador,&entidades.fantasmas);

    if (estado == DERROTA)
        submenuDerrota(&entidades.jugador);

    destruirLaberinto(&laberinto);
    destruirVector(&entidades.fantasmas);

    return EXITO;
}
void registroMov(tJugador *jugador, tVector *fantasmas)
{
    unsigned cantFantasmas = obtenerLongitudVector(fantasmas);
    tFantasma *fantasma;
    printf("\njugador\n");
    mostrarCola(&jugador->cola,imprimirPos);


    for(int i = 0; i < cantFantasmas; i++)
    {
        fantasma = obtenerElementoVector(fantasmas,i);
        printf("\nfantasma %d\n",i+1);
        mostrarCola(&fantasma->cola,imprimirPos);
    }

}

int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion * config)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    char casilla;
    int jugadorEncontrado = FALSO;
    tFantasma fantasmaAux;

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            casilla = obtenerCasillaLaberinto(laberinto, i, j);

            switch (casilla)
            {
            case ENTRADA:
                if (jugadorEncontrado) // Por si hay más de un jugador en el laberinto
                    break;

                crearJugador(&entidades->jugador, config, i, j);
                jugadorEncontrado = VERDADERO;
                break;
            case FANTASMA:
                crearFantasma(&fantasmaAux, i, j);
                insertarVector(&entidades->fantasmas, &fantasmaAux);
                modificarCasillaLaberinto(laberinto, i, j, CAMINO); // Sacamos la 'F' para que no moleste
                break;
            }
        }
    }

    return jugadorEncontrado == VERDADERO;
}


short int actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado)
{
    char teclaApretada;
    char direccionJugador = NO_DIRECCION;
    tFantasma *fantasma;
    short int estado = CONTINUA;
    int cantFantasmas = obtenerLongitudVector(&entidades->fantasmas);
    tPosicion pos;
    char mov;
    int i;

    // Acá habría que hacer que espere a que el jugador toque una tecla para mover al personaje

    if (_kbhit()) // _kbhit() ya viene con 'conio.h' y se traduce en keyboard hit
    {
        teclaApretada = _getch();

        if (teclaApretada == 'q')
            *juegoTerminado = VERDADERO;
        else
        {
            if (TECLA_ABAJO(teclaApretada))
                direccionJugador = ABAJO;
            else if (TECLA_ARRIBA(teclaApretada))
                direccionJugador = ARRIBA;
            else if (TECLA_IZQUIERDA(teclaApretada))
                direccionJugador = IZQUIERDA;
            else if (TECLA_DERECHA(teclaApretada))
                direccionJugador = DERECHA;


            // Solo si el jugador se mueve, los fantasmas tambi�n
            // Ac� en realidad habr�a que encolar el movimiento del jugador y el de los fantasmas y luego mover

            if (moverJugador(&entidades->jugador, direccionJugador, laberinto) == VERDADERO)
            {
                pos = obtenerPosJugador(&entidades->jugador);
                ponerEncola(&entidades->jugador.cola,&pos,sizeof(pos));

                // Ac� hay que hacer que se muevan los fantasmas
                for( i = 0; i < cantFantasmas; i++)
                {
                    fantasma = (tFantasma*)obtenerElementoVector(&entidades->fantasmas,i);
                    mov = calcularMovimientoFantasma(fantasma,laberinto,&entidades->jugador);
                    pos = obtenerPosFantasma(fantasma);

                    ponerEncola(&fantasma->cola,&pos,sizeof(pos));

                    moverFantasma(fantasma,mov,laberinto);
                // Para hacer: Comprobar si el jugador tocó un fantasma, vida extra o premio.

                if (chequeoSalida(&entidades->jugador, laberinto))
                {
                    *juegoTerminado = VERDADERO;
                    estado = VICTORIA;
                }

                if(chequeoPremio (&entidades->jugador, laberinto))
                {
                    sumarPuntaje(&entidades->jugador, laberinto);
                    ponerCamino(&entidades->jugador, laberinto);
                }

                if (chequeoVida(&entidades->jugador, laberinto))
                {
                    sumarVida(&entidades->jugador);
                    ponerCamino(&entidades->jugador, laberinto);
                }

                if(chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
                {
                    volverYDescontar(&entidades->jugador);

                    if (esFinPartida(&entidades->jugador))
                    {
                        *juegoTerminado = VERDADERO;
                        estado = DERROTA;
                    }


                }

            }
        }
    }
     return estado;
}

tPosicion obtenerPosJugador(tJugador *jugador)
{
    tPosicion pos;

    pos.fila = jugador->filaActual;
    pos.columna = jugador->columnaActual;

    return pos;

}
tPosicion obtenerPosFantasma(tFantasma *fantasma)
{
    tPosicion pos;

    pos.fila = fantasma->filaActual;
    pos.columna = fantasma->columnaActual;

    return pos;
}
void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            // Primero el jugador
            if (entidades->jugador.filaActual == i && entidades->jugador.columnaActual == j)
            {
                dibujarJugador(&entidades->jugador, i, j);
                continue;
            }

            // Recorremos el vector de fantasmas para ver si alguno está en esta fila y columna para dibujarlo
            if (hayFantasma(&entidades->fantasmas, i, j) == VERDADERO)
                continue;

            // Dibujamos el resto del laberinto
            printf("%c", obtenerCasillaLaberinto(laberinto, i, j));
        }

        puts("");
    }
    puts("");
    vidasYPuntos(&(entidades->jugador));
}

int hayFantasma(tVector* vecFantasmas, size_t fila, size_t columna)
{
    size_t i;
    tFantasma* fantasma;

    for (i = 0; i < obtenerLongitudVector(vecFantasmas); i++)
    {
        fantasma = (tFantasma*)obtenerElementoVector(vecFantasmas, i);

        if (fantasma->filaActual == fila && fantasma->columnaActual == columna && !fantasma->tocado)
        {
            dibujarFantasma(fantasma, fila, columna);
            return VERDADERO; // Salimos antes porque ya se dibujó un fantasma
        }
    }

    return FALSO;
}

void imprimirInt(const void* c)
{
     unsigned char p = *(unsigned char*)c;
    printf("%u ", (unsigned)p);
}

void imprimirPos(const void *p)
{
    tPosicion *pos = (tPosicion*)p;

    printf("(%u,%u)\t",pos->fila,pos->columna);

void volverYDescontar(tJugador * jugador)
{
    jugador->vidas--;
    jugador->filaActual = jugador->filaInicial;
    jugador->columnaActual = jugador->columnaInicial;
}

unsigned short esFinPartida (tJugador * jugador)
{
    return (jugador->vidas <= 0 ? VERDADERO : FALSO);
}

unsigned short chequeoSalida (tJugador * jugador, tLaberinto * laberinto)
{
    return (laberinto->casillas[jugador->filaActual][jugador->columnaActual] == SALIDA ? VERDADERO : FALSO);
}

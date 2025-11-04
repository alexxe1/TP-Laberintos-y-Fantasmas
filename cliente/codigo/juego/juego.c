#include "juego.h"

int empezarJuego(SOCKET* socket)
{
    unsigned char juegoTerminado = FALSO;
    char estado;
    unsigned iteracion = 1;
    char nombreJug[MAX_NOM];
    char esNomValido;
    int resultado;
    size_t cantMovimientos;
    char dioAltaJugador = FALSO;
    tLaberinto laberinto;
    tEntidades entidades;
    tConfiguracion configuracion;

    resultado = cargarArchivoConfiguracion(&configuracion);

    if(resultado == ERROR_ARCHIVO)
    {
        puts("ERROR: No se pudo procesar el archivo 'Config.txt' Verifique la sintaxis.\n");
        _getch();
        return ERROR;
    }
    if(resultado == ERROR_FILAS)
    {
        printf("ERROR: Cantidad de Filas invalida. MAX: %d -- MIN: %d\n", MAX_FILAS, MIN_FILAS);
        _getch();
        return ERROR;
    }
    if(resultado == ERROR_COLUMNAS)
    {
        printf("ERROR: Cantidad de Columnas invalida. MAX: %d -- MIN: %d\n", MAX_COLUMNAS, MIN_COLUMNAS);
        _getch();
        return ERROR;
    }
    if(resultado == ERROR_VIDAS_INICIALES)
    {
        printf("ERROR: Cantidad de Vidas iniciales invalida. MAX: %d -- MIN: 1\n", MAX_VIDAS_INICIALES);
        _getch();
        return ERROR;
    }
    if(resultado == ERROR_FANTASMAS)
    {
        printf("ERROR: Numero de Fantasmas invalido. MAX: %d -- MIN: 0\n", (int)(configuracion.filas * configuracion.columnas / DIV_MAX_FANTASMAS));
        _getch();
        return ERROR;
    }
    if(resultado == ERROR_PREMIOS)
    {
        printf("ERROR: Numero de Premios invalido. MAX: %d -- MIN: 0\n", (int)(configuracion.filas * configuracion.columnas / DIV_MAX_PREMIOS));
        return ERROR;
    }
    if(resultado == ERROR_VIDAS_EXTRAS)
    {
        printf("ERROR: Numero de Vidas Extra invalido. MAX: %d -- MIN: 0\n", (int)(configuracion.filas * configuracion.columnas / DIV_MAX_VIDAS_EXTRAS));
        _getch();
        return ERROR;
    }

    // Cargamos laberinto de un .txt
    // if (!crearLaberintoArchivo(&laberinto))
    //     return ERROR;

    // Inicializo semilla random
    srand((unsigned)time(NULL));

    // Generamos un laberinto aleatorio
    if (!crearLaberintoAleatorio(&laberinto, &configuracion, iteracion))
    {
        puts("ERROR: Hubo un error al crear el laberinto aleatorio");
        _getch();
        return ERROR;
    }

    // Inicializar el vector de fantasmas
    if (!crearVector(&entidades.fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(&laberinto);
        puts("ERROR: No hay memoria para crear los fantasmas");
        _getch();
        return ERROR;
    }

    do
    {
        system("cls");
        puts("Ingrese el nombre del jugador: ");
        ingresarNombre(nombreJug, MAX_NOM);

        esNomValido = esNombreValido(nombreJug);

        if (!esNomValido)
        {
            puts("\nNombre de usuario invalido.");
            puts("-> Solo se permiten letras, numeros y guiones bajos.");
            puts("-> El nombre debe empezar con una letra y no se permiten espacios.");
            puts("-> El nombre debe tener minimo 3 caracteres.");
            puts("-> El nombre solo va a tomar hasta 16 caracteres.\n");
            system("pause");
        }
    }
    while(!esNomValido);

    // Leemos el laberinto y lo interpretamos para generar todo
    if (!procesarEntidades(&laberinto, &entidades, &configuracion, nombreJug, FALSO))
    {
        destruirLaberinto(&laberinto);
        vaciarVector(&entidades.fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        _getch();
        return ERROR;
    }

    // Se intenta dar el alta del jugador en el servidor
    if (darAltaJugadorServidor(socket, entidades.jugador.nombre) == ERROR)
    {
        puts("Ocurrio un error al dar de alta al jugador en el servidor. Tu puntuacion no se guardara.");
        puts("Apreta cualquier tecla para continuar...");
        _getch();
    }
    else
    {
        dioAltaJugador = VERDADERO;
    }

    // Dibujado inicial
    dibujarJuego(&laberinto, &entidades);

    // Bucle principal
    while (!juegoTerminado)
    {
        estado = actualizarJuego(&laberinto, &entidades, &juegoTerminado);

        if (estado == CONTINUA)
        {
            dibujarJuego(&laberinto, &entidades);
        }

        if (estado == VICTORIA)
        {
            estado = submenuTransicion(&entidades.jugador, laberinto.nivel);

            // Le preguntamos al jugador si quiere irse al menú principal
            if (estado == VERDADERO)
            {
                juegoTerminado = VERDADERO;
            }
            else // O si quiere seguir
            {
                destruirLaberinto(&laberinto);
                vaciarVector(&entidades.fantasmas);
                iteracion++;

                if(!continuarJugando(&laberinto, &configuracion, &entidades, iteracion))
                {
                    destruirColasYVectores(&entidades);
                    return ERROR;
                }

                dibujarJuego(&laberinto, &entidades);
            }
        }

        Sleep(100);
    }

    // Mostramos y guardamos los movimientos del jugador y cuantos fueron
    cantMovimientos = mostrarMovimientos(&entidades.jugador);
    puts("\nApreta cualquier tecla para continuar...");
    getch();

    // Mostramos el resumen de la partida
    submenuDerrota(&entidades.jugador, laberinto.nivel);

    // Solo guardamos el puntaje si se dio alta en el servidor
    if (dioAltaJugador == VERDADERO)
    {
        system("cls");
        puts("Enviando tu puntaje al servidor...\n");

        // Intentamos mandar los datos de la partida al servidor para que los guarde
        if (mandarDatosPartidaServidor(socket, entidades.jugador.nombre, entidades.jugador.puntajeTotal, cantMovimientos) == ERROR)
        {
            puts("Ocurrio un error al enviar tu puntaje.");
            puts("\nApreta cualquier tecla para continuar...");
        }
        else
        {
            puts("Tu puntaje fue enviado correctamente!");
            puts("\nApreta cualquier tecla para continuar...");
        }
        getch();
    }

    destruirLaberinto(&laberinto);
    destruirColasYVectores(&entidades);

    return EXITO;
}

char actualizarJuego(tLaberinto* laberinto, tEntidades* entidades, unsigned char* juegoTerminado)
{
    char teclaApretada;
    char direccionJugador = NO_DIRECCION;
    tMov movimiento;

    if (!_kbhit()) // Si no se apretó nada, no perdemos tiempo en calcular nada
        return SIN_MOVIMIENTO;

    teclaApretada = _getch();

    if (teclaApretada == SALIR_DEL_JUEGO) // Si se va a cerrar el juego, tampoco deberíamos calcular nada más
    {
        *juegoTerminado = VERDADERO;
        return SIN_MOVIMIENTO;
    }

    // Determinamos la dirección del jugador
    if (TECLA_ABAJO(teclaApretada))
        direccionJugador = ABAJO;
    else if (TECLA_ARRIBA(teclaApretada))
        direccionJugador = ARRIBA;
    else if (TECLA_IZQUIERDA(teclaApretada))
        direccionJugador = IZQUIERDA;
    else if (TECLA_DERECHA(teclaApretada))
        direccionJugador = DERECHA;

    // Si apretó una tecla no contemplada, no se mueve
    if (direccionJugador == NO_DIRECCION)
        return SIN_MOVIMIENTO;

    movimiento.tipo = JUGADOR;
    movimiento.id = 0;
    movimiento.mov = direccionJugador;

    ponerEncola(&entidades->colaMov, &movimiento, sizeof(tMov));

    // Le mando la cola ya cargada con el movimiento del jugador ya que aca es donde se sabe para que lado se mueve
    procesarMovimientos(entidades, laberinto, juegoTerminado);

    if (jugadorEnSalida(&entidades->jugador, laberinto))
    {
        return VICTORIA;
    }

    if (chequeoPremio(&entidades->jugador, laberinto))
    {
        sumarPuntaje(&entidades->jugador);
        modificarCasillaLaberinto(laberinto, entidades->jugador.posActual.fila, entidades->jugador.posActual.columna, CAMINO);
    }

    if (chequeoVida(&entidades->jugador, laberinto))
    {
        sumarVida(&entidades->jugador);
        modificarCasillaLaberinto(laberinto, entidades->jugador.posActual.fila, entidades->jugador.posActual.columna, CAMINO);
    }

    // Segundo chequeo de fantasma, post movimiento de los mismos
    if(chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
    {
        volverYDescontar(&entidades->jugador);

        if (jugadorSinVidas(&entidades->jugador))
        {
            *juegoTerminado = VERDADERO;
            return DERROTA;
        }
    }

    return CONTINUA;
}

char procesarMovimientos(tEntidades* entidades, tLaberinto* laberinto, unsigned char* juegoTerminado)
{
    int i;
    char mov;
    tMov evento;
    tFantasma *fantasma;

    size_t cantFantasmas = obtenerLongitudVector(&entidades->fantasmas);

    while(sacarDeCola(&entidades->colaMov, &evento, sizeof(tMov)))
    {
        //me fijo si es el movimiento del jugador, si es asi entonces lo muevo y calculo los movimientos que deben hacer los fantasmas
        //para alcanzarlo
        if(evento.tipo == JUGADOR)
        {
            if(moverJugador(&entidades->jugador, evento.mov, laberinto) == FALSO)
            {
                return CONTINUA;
            }

            ponerEncola(&entidades->jugador.colaMovimientos,&entidades->jugador.posActual,sizeof(tPosicion));

            if (chequeoFantasma(&entidades->fantasmas, &entidades->jugador))
            {
                volverYDescontar(&entidades->jugador);

                if (jugadorSinVidas(&entidades->jugador))
                {
                    *juegoTerminado = VERDADERO;
                    return DERROTA;
                }
            }

            //calculo el movimiento de los fantasmas y le asigno su identificador y el movimiento que realiza a la cola
            for (i = 0; i < cantFantasmas; i++)
            {

                fantasma = (tFantasma*)obtenerElementoVector(&entidades->fantasmas,i);
                mov = calcularMovimientoFantasma(fantasma,laberinto,&entidades->jugador);

                evento.id = i;
                evento.tipo = FANTASMA;
                evento.mov = mov;

                if (fantasma->tocado == FALSO)
                {
                    ponerEncola(&entidades->colaMov, &evento, sizeof(tMov));
                }
            }
            //si es un fantasma entonces el id nos da la posicion que tiene en el vector y movemos a ese fantasma
        }
        else if(evento.tipo == FANTASMA)
        {
            fantasma = (tFantasma*)obtenerElementoVector(&entidades->fantasmas, evento.id);
            moverFantasma(fantasma,evento.mov,laberinto);
        }
    }
    return CONTINUA;
}

size_t mostrarMovimientos(tJugador* jugador)
{
    tPosicion pos;
    int contador = 0;

    printf("\nHISTORIAL DE MOVIMIENTOS REALIZADOS...\n");

    while(!colaVacia(&jugador->colaMovimientos))
    {
        sacarDeCola(&jugador->colaMovimientos,&pos, sizeof(tPosicion));
        contador++;
        printf("(%lu,%lu)", (unsigned long)pos.fila, (unsigned long)pos.columna);
    }

    return contador;
}

int procesarEntidades(tLaberinto* laberinto, tEntidades* entidades, tConfiguracion* configuracion, const char* nombJug, unsigned iteracion)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    char casilla;
    int jugadorEncontrado = FALSO;
    tFantasma fantasmaAux;

    crearCola(&entidades->colaMov);

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

                if (iteracion <= 1)
                {
                    crearJugador(&entidades->jugador, nombJug, configuracion, i, j);
                }
                else
                {
                    acomodarJugador(&entidades->jugador, i,j);
                }


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

void dibujarJuego(tLaberinto* laberinto, tEntidades* entidades)
{
    size_t i, j;
    size_t filasLaberinto = obtenerFilasLaberinto(laberinto);
    size_t columnasLaberinto = obtenerColumnasLaberinto(laberinto);
    tFantasma* fantasmaEncontrado;
    int indiceFantasmaEncontrado = -1;

    system("cls");

    for (i = 0; i < filasLaberinto; i++)
    {
        for (j = 0; j < columnasLaberinto; j++)
        {
            // Primero el jugador
            if (entidades->jugador.posActual.fila == i && entidades->jugador.posActual.columna == j)
            {
                dibujarJugador(&entidades->jugador, i, j);
                continue;
            }

            // Recorremos el vector de fantasmas para ver si alguno está en esta fila y columna para dibujarlo
            indiceFantasmaEncontrado = buscarFantasmaEnPosicion(&entidades->fantasmas, i, j);

            if (indiceFantasmaEncontrado != -1)
            {
                fantasmaEncontrado = (tFantasma*)obtenerElementoVector(&entidades->fantasmas, indiceFantasmaEncontrado);
                dibujarFantasma(fantasmaEncontrado, i, j);
                continue;
            }

            // Dibujamos el resto del laberinto
            printf("%c", obtenerCasillaLaberinto(laberinto, i, j));
        }

        puts("");
    }

    puts("");
    mostrarVidasYPuntos(&entidades->jugador);
}

void imprimirPosicion(const void *p)
{
    tPosicion* pos = (tPosicion*)p;
    printf("(%u, %u)\t", (unsigned)pos->fila, (unsigned)pos->columna);
}

char continuarJugando (tLaberinto * laberinto, tConfiguracion * configuracion, tEntidades * entidades, unsigned nivel)
{
    if (!crearLaberintoAleatorio(laberinto, configuracion, nivel))
    {
        puts("ERROR: Hubo un error al crear el laberinto aleatorio");
        return ERROR;
    }

    if (!crearVector(&entidades->fantasmas, sizeof(tFantasma)))
    {
        destruirLaberinto(laberinto);
        puts("ERROR: No hay memoria para crear los fantasmas");
        return ERROR;
    }

    if (!procesarEntidades(laberinto, entidades, configuracion, NULL, nivel))
    {
        destruirLaberinto(laberinto);
        vaciarVector(&entidades->fantasmas);
        puts("ERROR: No se encontro una entrada en el laberinto");
        return ERROR;
    }

    return EXITO;
}

void destruirColasYVectores(tEntidades* entidades)
{
    destruirJugador(&entidades->jugador);

    for (size_t i = 0; i < obtenerLongitudVector(&entidades->fantasmas); i++)
    {
        destruirFantasma((tFantasma*)obtenerElementoVector(&entidades->fantasmas, i));
    }

    vaciarVector(&entidades->fantasmas);
    vaciarCola(&entidades->colaMov);
}

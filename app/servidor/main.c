#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include "utils.h"

#define MAX_ENQUEUED 20
#define PORT_STR "7777"

/**
 * Configura el servidor eco, para recibir y respoder solicitudes en texto plano.
 * @warn Este programa sólo está pensado para correr en Linux, no ha sido probado en Windows u en otro sistema.
 * @param argc Cantidad de Argumentos
 * @param argv Matriz asociativa con los argumentos
 * @return El estado de salida del programa.
 */
int main(int argc, char** argv) {
    /* Se definen estructuras para manejar las conexiones. */
    /* La estrucutra que escuchará las peticiones */
    struct addrinfo escuchador;
    /* La estructura que responderá las peticiones */
    struct addrinfo *respondedor;
    /*  Estructura para el manejo de señales. */
    struct sigaction sa;
    int sock;

    /* Limipamos la estructura a usar con 0 */
    memset(&escuchador, 0, sizeof (struct addrinfo));
    /*
     * Familia de protocolos a los que responderá el Socket
     */
    escuchador.ai_family = AF_UNSPEC;

    /*
     * Tipo de Socket
     */
    escuchador.ai_socktype = SOCK_STREAM;
    /* 
     * Indicadores que modifican el comportamiento del socket
     */
    escuchador.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, PORT_STR, &escuchador, &respondedor) != 0) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error al levantar el puerto en la máquina local
         */
        perror("getaddrinfo");
        /*
         * El programa termina con error
         */
        exit(EXIT_FAILURE);
    }

    /*
     * Realiza la conexión del socket
     */
    if ((sock = socket(respondedor->ai_family, respondedor->ai_socktype, respondedor->ai_protocol)) == -1) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error establecer la conexión del socket
         */
        perror("socket");
        /*
         * El programa termina con error
         */
        exit(EXIT_FAILURE);
    }

    /*
     * Se levanta el manejador de conexiones muertas (zombies).
     */
    sa.sa_handler = esparar_zombies;

    /*
     * Se procesa la señal que manejará la aplicación.
     */
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error por una señal no procesada por el sistema operativo.
         */
        perror("sigaction");
        /*
         * El programa termina con error
         */
        exit(EXIT_FAILURE);
    }

    /*
     * Se asocia el socket con su puerto.
     */
    if (bind(sock, respondedor->ai_addr, respondedor->ai_addrlen) != 0) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error por la incapacidad de asociar el puerto.
         */
        perror("bind");
        /*
         * El programa termina con error
         */
        exit(EXIT_FAILURE);
    }
    /*
     * Se liberan los recursos asociados a la respuesta.
     */
    freeaddrinfo(respondedor);


    /*
     * Se levanta un estado en el cual establecemos la cantidad de conexiones encoladas.
     */
    if (listen(sock, MAX_ENQUEUED) != 0) {
        /* 
         * Muestra un mensaje correspondiente a un error del sistema, en 
         * particular un error por llegar al límite de clientes encolados.
         */
        perror("listen");
        /*
         * El programa termina con error
         */
        exit(EXIT_FAILURE);
    }

    /*
     * Se escuchan conexiones hasta que el programa sea terminado (a la mala).
     */
    escuchar_conexiones(sock);
    return EXIT_SUCCESS;
}

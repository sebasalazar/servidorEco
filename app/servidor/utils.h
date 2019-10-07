#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

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

#define BUF_LEN 256

    /**
     * Indica como limpiar los procesos hijos que han 'muerto'
     * @param socket Identificador de socket
     */
    void esparar_zombies(int socket);

    /**
     * Implementación base que responde con lo mismo que se le pregunta.
     * @param socket_cliente Socket del cliente
     */
    void responder_lo_mismo(int socket_cliente);

    /**
     * Proceso que atienda las conexiones entrantes
     * @param socket_servidor
     */
    void escuchar_conexiones(int socket_servidor);

#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */


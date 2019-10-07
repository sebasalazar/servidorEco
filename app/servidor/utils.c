#include "utils.h"

void esparar_zombies(int socket) {
    while (waitpid(-1, NULL, WNOHANG) > 0);
}

void responder_lo_mismo(int socket_cliente) {
    char buf[BUF_LEN];
    int r;

    while ((r = read(socket_cliente, buf, BUF_LEN)) > 0) {
        (void) write(socket_cliente, buf, r);
    }
    exit(EXIT_SUCCESS);
}

void escuchar_conexiones(int socket_servidor) {
    for (;;) {
        struct sockaddr addr;
        socklen_t addr_size = sizeof (addr);
        int socket_cliente;

        /*
         * Se bloquea hasta que el servidor tome una conexión. 
         * 
         */
        socket_cliente = accept(socket_servidor, &addr, &addr_size);

        /*
         * Si la conexión es exitosa se levanta un proceso que lo atienda
         */
        if (socket_cliente == -1) {
            perror("accept");
        } else if (fork() == 0) {
            close(socket_servidor);
            responder_lo_mismo(socket_cliente);
        } else {
            close(socket_cliente);
        }
    }
}
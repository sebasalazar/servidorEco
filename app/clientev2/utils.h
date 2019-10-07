#ifndef UTILS_H
#define UTILS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netdb.h>


    /**
     * Obtiene una ip desde un nombre de servidor.
     * @param hostname nombre del servidor
     */
    char* get_ip(char *hostname);


#ifdef __cplusplus
}
#endif

#endif /* UTILS_H */


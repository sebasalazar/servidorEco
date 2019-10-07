#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

/**
 * 
 * @param argc Cantidad de Argumentos
 * @param argv Matriz asociativa con los argumentos.
 * @return El estado de salida de la ejecución del programa.
 */
int main(int argc, char** argv) {

    const char* nombre_servidor = "localhost";
    const int puerto_servidor = 7777;

    struct sockaddr_in servidor;
    memset(&servidor, 0, sizeof (servidor));
    servidor.sin_family = AF_INET;

    /*
     * Crear una representación binaria del nombre del servidor y lo deja en la estructura.
     * @see http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
     */
    inet_pton(AF_INET, nombre_servidor, &servidor.sin_addr);

    /*
     * La función htons transforma el puerto a formato socket
     */
    servidor.sin_port = htons(puerto_servidor);

    /*
     * Se abre el socket
     */
    int sock;
    if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        /*
         * Se muestra un mensaje de error.
         */
        fprintf(stderr, "No fue posible crear el socket\n");
        /*
         * El programa termina con error.
         */
        return EXIT_FAILURE;
    }

    /*
     * Como el socket es TCP, la conexión debe establecerse primero,
     * antes de compartir datos.
     */
    if (connect(sock, (struct sockaddr*) &servidor,
            sizeof (servidor)) < 0) {
        /*
         * Se muestra un mensaje de error.
         */
        fprintf(stderr, "No se pudo establecer la comunicacion\n");
        /*
         * El programa termina con error
         */
        return EXIT_FAILURE;
    }

    /*
     * Preparación de datos a enviar
     * Datos a enviar
     */
    const char* mensaje = "Hola mundo";
    int largo_mensaje = strlen(mensaje);
    send(sock, mensaje, largo_mensaje, 0);


    /*
     * Datos para recibir.
     */
    int n = 0;
    int largo = 0;
    int largo_recibido = (largo_mensaje + 1);
    char buffer[largo_recibido];
    char* pbuffer = buffer;

    /*
     * Estaremos esperando datos.
     */
    while ((n = recv(sock, pbuffer, largo_recibido, 0)) > 0) {
        pbuffer += n;
        largo_recibido -= n;
        largo += n;

        /*
         * Fin de linea
         */
        buffer[largo] = '\0';
        fprintf(stdout, "Se ha recibido: '%s'\n", buffer);
    }

    /*
     * Cerramos el socket
     */
    close(sock);

    /*
     * Terminamos el programa
     */
    return EXIT_SUCCESS;
}
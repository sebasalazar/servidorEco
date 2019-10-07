#include "utils.h"

char* get_ip(char *hostname) {
    char *ip = (char *) calloc(20, sizeof (char));
    struct hostent* host;
    host = gethostbyname(hostname);
    //    snprintf(ip, 20, "%s", inet_ntoa(host->h_addr));
    snprintf(ip, 20, "%s", inet_ntoa(*(struct in_addr *) host->h_name));
    fprintf(stdout, "%s", ip);
    return ip;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <netdb.h> // per le richieste
#define BUFFER_SIZE 4096


int main(int argc, char** argv) {

    if (argc < 3) {
        fprintf(stderr, "Uso: %s <host> <port>\n", argv[0]);
        exit(1);
    }
    
    char *host = argv[1]; // ip host
    struct hostent *server_host; // struttura dati che caratterizza l'host

    char request[1024], response[BUFFER_SIZE]; // richiesta e buffer per la risposta
    int port = atoi(argv[2]); // porta host

    int sock; // socket (intero)
    struct sockaddr_in server;  // struttura dati per la connessione

    char buffer[1024];
    

    sock = socket(AF_INET, SOCK_STREAM, 0); // setup socket per tcp/ip
    if (sock < 0) { // gestione errori
        perror("[-] Socket error");
        exit(-1);
    }

    server_host = gethostbyname(host);

    if(server_host== NULL) 
    {
        perror("[-] c'é stato un problema");
        exit(-1);
    }

    printf("[+] TCP client [http] socket created\n");

    memset(&server,  0, sizeof(server) ); // setto la stringa a 0
    server. sin_family = AF_INET; // tcp/ip
    server. sin_port = htons(port); // converto la porta in byte
    memcpy(&server.sin_addr.s_addr, server_host->h_addr_list[0], server_host->h_length);

    if(connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0){
        perror("[-] connessione non riuscita");
        exit(-1);
    }

    printf("[+] connessione stabilita\n");

    snprintf(request, sizeof(request),
        "GET / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n\r\n",
        host
    
    ); // la funzione mi "stampa" una stringa nel buffer della mia variabile

    printf("connected to the server %s\n\n\n", host); // connesso 

    if (send(sock, request, strlen(request), 0) < 0) {
    perror("Errore nell'invio della richiesta");
    exit(-1);
    }   

    printf("Richiesta inviata:\n%s\n", request);
   
    int byte_ricevuti;
    memset(response, 0, sizeof(response)); // formato la stringa a 0 per evitare problemi di encoding con il \0000

    printf("Risposta del server: \n\n");
    while ((byte_ricevuti = recv(sock, response, sizeof(response) - 1, 0)) > 0) {
        response[byte_ricevuti] = '\0'; // Assicurarsi che la stringa sia terminata
        printf("%s", response);
    }
    

   
    close(sock);



    printf("\n[+] disconnected from the server\n");

    return 0;
}

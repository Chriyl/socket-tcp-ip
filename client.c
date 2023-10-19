#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main() {
    char *ip = "127.0.0.1";
    int port = 5007;

    int sock;
    struct sockaddr_in addr;

    char buffer[1024];
    int n;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("[-] Socket error");
        exit(1);
    }

    printf("[+] TCP client socket created\n");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port); // Use htons to convert port to network byte order
    addr.sin_addr.s_addr = inet_addr(ip);

    connect(sock, (struct sockaddr *) &addr, sizeof(addr));
    printf("connected to the server\n");

    for(;;) {
        bzero(buffer, 1024);
        printf("inserisci un messagio: ");
        fgets(buffer, 1024, stdin);

        

        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0'; // Sostituisci '\n' con '\0'
        }

        //printf("%ld\n", send(sock, buffer, strlen(buffer), 0));

        send(sock, buffer, strlen(buffer), 0);

        if(strcmp(buffer, "exit") == 0) {
            close(sock);
            printf("[+] TCP connection closed");
            return 0;
        }

        bzero(buffer, 1024);
        recv(sock, buffer, sizeof(buffer), 0);
        printf("server: %s \n", buffer);

    }

   
    close(sock);
    printf("[+] disconnected from the server\n");

    return 0;
}

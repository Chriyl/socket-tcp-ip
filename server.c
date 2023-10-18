#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdbool.h>

int main() {
    char *ip = "127.0.0.1";
    int port = 5007;

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;

    char *prova = "anche a te";
    char trash[1024];
    char buffer[1024];
    int n;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("[-] Socket error");
        exit(1);
    }

    printf("[+] TCP server socket created\n");

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); // Use htons to convert port to network byte order
    server_addr.sin_addr.s_addr = inet_addr(ip);

    n = bind(server_sock, (struct sockaddr *) &server_addr, sizeof(server_addr));
    if (n < 0) {
        perror("[-] Bind error");
        exit(1);
    }

    printf("[+] bind to the port number: %d\n", port);

    listen(server_sock, 5);
    printf("Listening...\n");

    while (true) {
        int addr_size = sizeof(client_addr);
        client_sock = accept(server_sock, (struct sockaddr *) &client_addr, &addr_size);
        printf("[+] client connected\n");

        bzero(buffer, 1024);

        recv(client_sock, buffer, sizeof(buffer), 0);
        
        printf("client: %s \n", buffer);

        //bzero(buffer, 1024);
        strcat(buffer, " anche a te");   
        

       
        
        send(client_sock,  buffer, strlen(buffer), 0);
       

        close(client_sock);
        
        printf("[+] client close\n");

        close(server_sock); // Close the server socket when done
        printf("[+] server close");
        return 0;
        
    }

    
    return 0;
}

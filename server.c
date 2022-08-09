#include<sys/socket.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include <string.h>
#include<strings.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>

#include "signup.h"
#include "login.h"

// struct account{
//     int id;
//     char name[10];
//     char pass[30];
// };

void take_options(int sock);

int main(){
    struct sockaddr_in server, client;
    socklen_t clen;
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    perror("socket");

    int optval = 1;
	int optlen = sizeof(optval);
	if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &optval, optlen)==-1){
		printf("set socket options failed\n");
		exit(0);
	}

    server.sin_family = AF_INET;
    server.sin_port = htons(6050);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sd, (struct sockaddr*)&server, sizeof(server));
    perror("bind");

    listen(sd,100);
    perror("listen");

    while(1)
    {
        clen = sizeof(client);

        int accpt = accept(sd, (struct sockaddr*)&client, &clen);
        perror("accept");

        int pid = fork();

        if(pid == 0)
        {
            take_options(accpt);
        }
    }

    close(sd);

    return 0;
}

void take_options(int sock){
    int fid;
    printf("REading fid\n");
    read(sock, &fid, sizeof(int));
    printf("Client connected: %d\n",sock);

    while(1)
    {
        if(fid == 1){
            login(sock);
            read(sock,&fid, sizeof(int));
        }

        if(fid == 2){
            printf("Im here\n");
            signup(sock);
            read(sock, &fid, sizeof(int));
        }

        if(fid == 3)
            break;
    }
    close(sock);
    printf("Client disconnected %d\n",sock);
}


//signup and login in another file
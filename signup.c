#include "signup.h"

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

char *path[3] = {"./customer.txt", "./agents.txt", "./admin.txt"};


int signup(int sock){
    printf("In signup\n");
    int type;
    char name[20], password[30];
    int acc_no = 0;

    struct account temp;

    read(sock, &type, sizeof(type));
    read(sock, &name, sizeof(name));
    read(sock, &password, sizeof(password));

    // const char *path = "/home/anon/Documents/SS Lab-Handson 2/Mini_project/file1.txt";
    int fd;
    fd = open(path[type-1], O_CREAT|O_RDWR,00400|00200);
    perror("open");

    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = 0;
    lock.l_len = 0;
    lock.l_whence = SEEK_SET;
    lock.l_pid = getpid();

    fcntl(fd,F_SETLKW, &lock);

    int fp = lseek(fd, 0, SEEK_END);

    if(fp == 0){
        printf("temp id noted\n");
        temp.id = 1;
        strcpy(temp.name, name);
        strcpy(temp.pass, password);

        write(fd, &temp, sizeof(temp));
        
        write(sock, &temp.id, sizeof(temp.id));
        write(sock, &temp.name, sizeof(temp.name));
    }
    else{
        printf("created\n");
        fp = lseek(fd, -1 * sizeof(struct account), SEEK_END);
        read(fd, &temp, sizeof(temp));
        temp.id++;
        strcpy(temp.name, name);
        strcpy(temp.pass, password);

        write(fd, &temp, sizeof(temp));

        write(sock, &temp.id, sizeof(temp.id));       
        write(sock, &temp.name, sizeof(temp.name)); 
    }

    lock.l_type = F_UNLCK;

    fcntl(fd, F_SETLK, &lock);
    close(fd);

    return 3;
}

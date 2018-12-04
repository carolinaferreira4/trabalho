#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <ncurses.h>

#include "Estruturas.h"


void sentLogin(); 
void sentRequest(); 

int main(int argv[], int argc){
    
    sentLogin();
    sentRequest();
    
}

void sentLogin() {
    int fd, fp;
    int answer;
    char fifo[50];
    Client c;
    
    sprintf(fifo,FIFOCLI, getpid());
    mkfifo(fifo, 0660);

    //LOGIN
    fd = open(FIFOLOGIN, O_WRONLY);
    if(fd == -1){
      printf("Erro ao abrir fifo"); 
      exit(1);
    }
    
    printf("Nome: ");
    scanf("%[^\n]", c.username);
    c.PID = getpid();
    printf("%s - %d\n", c.username, c.PID);

    write(fd, &c, sizeof(c));

    fp = open(fifo, O_RDONLY);
    read(fp, &answer, sizeof(answer));

    if(answer == 1) 
        printf("Utilizador aceite");
    else
        printf("Utilizador nao existente na base de dados");
    
    close(fd);
}

void sentRequest() {
    int fd, fp;
    int answer;
    char fifo[50];
    Request r;
    
    fd = open(FIFOREQUEST, O_WRONLY);
    if(fd == -1) {
      printf("Erro ao abrir fifo");
      exit(1);
    }
    
    printf("Linha a editar: ");
    scanf("%[^\n]", r.line);
    r.PID = getpid();
    printf("%d - %d\n", r.line, r.PID);
    
    write(fd, &r, sizeof(r));
    
    sprintf(fifo,FIFOCLI, getpid()); 
    mkfifo(fifo, 0600);
    
    fp = open(FIFOCLI, O_RDONLY);
    
    if(read(fp, &answer, sizeof(answer)) == sizeof(answer)) {
        if(answer == 1) 
            printf("Ediçao possivel");
        else
            printf("Ja existe alguem a editar esta linha");
    }
    close(fd);
}



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
//#include <ncurses.h>

#include "Estruturas.h"


int main(int argv[], int argc){
    int fl, fp, fr;
    int asw, answer;
    char fifo[50];
    Client c;
    Request r;

    sprintf(fifo,FIFOCLI, getpid());
    mkfifo(fifo, 0660);

    //LOGIN
    fl = open(FIFOLOGIN, O_WRONLY);
    if(fl == -1){
      printf("Erro ao abrir fifo"); 
      exit(1);
    }
    
    printf("Nome: ");
    scanf("%[^\n]", c.username);
    c.PID = getpid();
    printf("%s - %d\n", c.username, c.PID);

    write(fl, &c, sizeof(c));

    fp = open(fifo, O_RDONLY);
    read(fp, &asw, sizeof(asw));

    if(asw == 1) 
        printf("Utilizador aceite");
    else
        printf("Utilizador nao existente na base de dados");

    //REQUEST
    fr = open(FIFOREQUEST, O_WRONLY);
    if(fr == -1){
      printf("Erro ao abrir fifo");
      exit(1);
    }
    
    printf("Linha a editar: ");
    scanf("%[^\n]", r.line);
    r.PID = getpid();
    printf("%d - %d\n", r.line, r.PID);
    
    write(fr, &r, sizeof(r));
    
    read(fp, &answer, sizeof(answer));

    if(answer == 1) 
        printf("Ediçao possivel");
    else
        printf("Ja existe alguem a editar esta linha");
  
    close(fl);
    close(fr);
}



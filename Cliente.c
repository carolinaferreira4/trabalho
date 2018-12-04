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

typedef struct l{
    
    Line fullLine;
    struct l *next;
} Lines;

Lines *lines;
        
int dealsLogin(); 
void dealsRequest(); 
int ReceiveInitialData();
void clearStdin();

int main(int argv[], int argc){
    
    char fifo[50];
    int temp;
    lines = NULL;
    WINDOW * mainwin;
    
    sprintf(fifo, FIFOCLI, getpid());
    mkfifo(fifo, 0600);
    
    dealsLogin();
    
   /* if ((mainwin = initscr()) == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }*/
    
    /*noecho();
    keypad(mainwin, TRUE);
    curs_set(0);
    start_color();*/
    
    
    
}

int dealsLogin() {
    int fd, fp;
    Line answer;
    Client c;
    
    
    fd = open(FIFOLOGIN, O_WRONLY);
    if(fd == -1){
      printf("Erro ao abrir fifo"); 
      exit(1);
    }
    
    do{
        printf("Nome: ");
        scanf("%[^\n]", c.username);
        clearStdin();
        c.PID = getpid();
        printf("%s - %d\n", c.username, c.PID);

        write(fd, &c, sizeof(c));


        if(ReceiveInitialData() == 1){
            close(fd);
            break;
        } else{
            printf("Utilizador nao registado.\n");
        }
    }while(1);
}

void dealsRequest() {
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
    scanf("%d", r.line);
    r.PID = getpid();
    printf("%d - %d\n", r.line, r.PID);
    
    write(fd, &r, sizeof(r));
    
    sprintf(fifo,FIFOCLI, getpid()); 
    
    
    fp = open(FIFOCLI, O_RDONLY);
    
    if(read(fp, &answer, sizeof(answer)) == sizeof(answer)) {
        if(answer == 1) 
            printf("Ediçao possivel");
        else
            printf("Ja existe alguem a editar esta linha");
    }
    close(fd);
}

int ReceiveInitialData()
{
    int fd;
    char fifo[50];
    Line answer;
    Lines *end, *new;
    
    sprintf(fifo, FIFOCLI, getpid());
    fd = open(fifo, O_RDWR);
    if(fd == -1){
      printf("Erro ao abrir fifo"); 
      fflush(stdout);
      exit(1);
    }

    do{
        read(fd, &answer, sizeof(answer));

        if(answer.c.PID == -2)
        {
            close(fd);
            return -1;
        }else {
            if(answer.c.PID == -3)
            {
                close(fd);
                return 1;
            } else {
                //COLOCA TODA A INFORMAÇAO DAS LINHAS NA LISTA LIGADA "LINES" PARA NCURSES SER POSSIVEL 
                if(lines == NULL)                                                   //LISTA VAZIA
                {
                    lines = (Lines*) malloc(sizeof(Lines));
                    strcpy(lines->fullLine.nLine, answer.nLine);
                    lines->fullLine.c.PID = answer.c.PID;
                    strcpy(lines->fullLine.c.username, answer.c.username);
                    lines->next = NULL;
                    end = lines;
                } else {                                                            //ACRESCENTA
                    new = (Lines*) malloc(sizeof(Lines));
                    strcpy(new->fullLine.nLine, answer.nLine);
                    new->fullLine.c.PID = answer.c.PID;
                    strcpy(new->fullLine.c.username, answer.c.username);
                    new->next = NULL;
                    end->next = new;
                    end = new;
                }
            }
        }
        
    }while(1);
}

// IMPEDE O WHILE NA MAIN DE ENTRAR EM LOOP
void clearStdin() {
    int c;

    do {
        c = getchar();
    } while(c!='\n' && c!= EOF);
}

void show()
{
    Lines *it = lines;
    
    /*while(it != NULL){
        printf("%s\n",it->fullLine.nLine);
        it = it->next;
    }*/
}
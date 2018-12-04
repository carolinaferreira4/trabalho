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

int sair = 0;
lClient * lclients;
Info info;

void clearStdin();
void settings();
void *clientLogin (void* info);
int verifyClient(Client c);
void *correctingPhrases (void* info);
void getInfoFromTXT();
void sendLines(int pid);

int main(int argv[], int argc){
    char command[50], line[100], text;
    char* token;
    char* phrase;
    pthread_t login;
    int i = 1, j;
    FILE *fd;

    lclients = (lClient*) malloc(sizeof(lClient));
    lclients->next = NULL;
    strcpy(lclients->username, "andre");
    
    getInfoFromTXT();

    //VARIAVEIS DO AMBIENTE
    const char* lin = getenv("MEDIT_MAXLINES");
    const char* col = getenv("MEDIT_MAXCOLUMNS");
    
    if(lin == NULL)
        info.maxLines = 15;
    else
        info.maxLines = atoi(lin);

    if(col == NULL)
        info.maxColumns = 45;
    else
        info.maxColumns = atoi(col);
    
    //VETOR DE LINHAS COM PONTEIROS A APONTAR CADA FRASE DE CADA LINHA
    info.fullText = (Line*)malloc(sizeof(Line) * info.maxLines);
    

    for(j = 0; j < info.maxLines; j++) {
        strcpy(info.fullText[j].nLine, " ");
    }

    pthread_create(&login, NULL, &clientLogin, NULL);
    
    //PROCESSA COMANDOS
    while(1) {
        printf("\nComando: ");
        scanf("%[^\n]", command);
        clearStdin();

        //GUARDA COMO TOKEN TUDO O QUE ESTIVER ANTES DO " "
        token = strtok(command, " ");

        //PERCORRE O RESTO DOS TOKENS
        while(token!= NULL) {
            if(strcmp(token, "settings") == 0) {
                settings();
            } else if(strcmp(token, "load") == 0) {
                token = strtok(NULL, " ");

                fd = fopen(token, "r");

                if(fd == NULL) {
                    printf("ERRO ao abrir ficheiro %s", token);
                    continue;
                }

                while(fread(line, 1, sizeof(line), fd) > 0) {                   //LE A LINHA DE TEXTO DO FICHEIRO
                    printf("%s\n", line);
                    printf("\nLoad efectuado com sucesso.");
                    strcpy(info.fullText[i].nLine, line);                             //COPIA A LINHA PARA A VARIAVEL FULL TEXT
                    i++;

                }

                fclose(fd);

            } else if(strcmp(token, "text") == 0)  {
                token = strtok(NULL, " ");

                for(j = 0; j < info.maxLines; j++) {
                    printf("%s\n", info.fullText[j].nLine);
                }
                continue;
            }
            token = strtok(NULL, " ");
        }
    }

    return 1;
}

// IMPEDE O WHILE NA MAIN DE ENTRAR EM LOOP
void clearStdin() {
    int c;

    do {
        c = getchar();
    } while(c!='\n' && c!= EOF);
}

void settings() {
    Info info;

    printf("\n-----Settings-----");
    printf("\nNumero de linhas: %d", info.maxLines);
    printf("\nNumero de colunas: %d", info.maxColumns);
    //....
}

void *clientLogin (void* info) {
    char str[80];
    int fd, fd_answer, fp;
    Client c;
    Line no_login;

    no_login.c.PID = -2;
    strcpy(no_login.nLine, "");
    mkfifo (FIFOLOGIN, 0660);

    fd = open(FIFOLOGIN, O_RDWR);
    if(fd == -1) {
      printf("Erro ao abrir o fifo");
      fflush(stdout);
      exit(1);
    }

    while(sair == 0) {
        if((read(fd, &c, sizeof(c))) == sizeof(c)) {
            if(verifyClient(c) == 1)
            {
                sendLines(c.PID);
                
            } else {   
               
                sprintf(str, FIFOCLI, c.PID);
                fp = open(str, O_WRONLY);
                if(fp == -1) {
                  printf("Erro ao abrir o fifo");
                  fflush(stdout);
                  exit(1);
                }
                write(fp, &no_login, sizeof(no_login));
                close(fd);
            }
        }
    }
}

int verifyClient(Client c) {
  lClient *it;

  if(lclients == NULL) {
      lclients = (lClient*) malloc(sizeof(lClient));
  } else {
    it = lclients;
    
    while(it != NULL) {
      if(strcmp(it->username,c.username) == 0)
        return 1;
      
      it = it->next;
    }
    printf("TCHAU!!!");
    fflush(stdout);
    return -1;
  }
}

void getInfoFromTXT() {
    char name[8];
    lClient* c;
    FILE* f;
    
    f = fopen("username.txt", "r");
    
    if(f == NULL) {
        printf("ERRO");
        exit(0);
    }
    
    while(fscanf(f, "%s", name) == 1) {
        //GUARDA A INFORMAÇAO
        c = (lClient*)malloc(sizeof(lClient));
        strcpy(c->username, name);
        c->PID = 0;
        c->next = NULL;
        
        //PASSA PARA A LISTA LIGADA
        if(lclients == NULL)
            lclients = c;
        else {
            c->next = lclients;
            lclients = c;
        }
    }
}


void sendLines(int pid)
{
     char str[80];
    int fd;
    
    
    sprintf(str, FIFOCLI, pid);
    
    fd = open(str, O_WRONLY);
    if(fd == -1) {
      printf("Erro ao abrir o fifo");
      fflush(stdout);
      exit(1);
    }

    for(int i=0;i<info.maxColumns; i++)
    {
        write(fd, &info.fullText[i], sizeof(Line));
    }
    close(fd);
}
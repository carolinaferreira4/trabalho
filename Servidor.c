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

void clearStdin();
void settings();
void *clientLogin (void* info);
int verifyClient(Client c);
void *correctingPhrases (void* info);

int main(int argv[], int argc){
    char command[50], line[100], text;
    char* token;
    char* phrase;
    pthread_t login;
    int i = 1, j;
    FILE *fd;
    Info info;
    //??? inf;

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
    info.fullText = (char**)malloc(sizeof(char*) * info.maxLines);

    for(j = 0; j < info.maxLines; j++) {
        info.fullText[j] = (char*)malloc(sizeof(char*) * info.maxColumns);
    }

    for(j = 0; j < info.maxLines; j++) {
        strcpy(info.fullText[j], "  ");
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
                    strcpy(info.fullText[i], line);                             //COPIA A LINHA PARA A VARIAVEL FULL TEXT
                    i++;

                }

                fclose(fd);

            } else if(strcmp(token, "text") == 0)  {
                token = strtok(NULL, " ");

                for(j = 0; j < info.maxLines; j++) {
                    printf("%s\n", info.fullText[j]);
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
    int fd, fd_answer, answer;
    Client c;

    mkfifo (FIFOLOGIN, 0660);

    fd = open(FIFOLOGIN, O_RDWR);
    if(fd == -1) {
      printf("Erro ao abrir o fifo");
      fflush(stdout);
      exit(1);
    }

    while(sair == 0) {
        if((read(fd, &c, sizeof(c))) == sizeof(c)) {
            sprintf(str, "../JJJ%d", c.PID);
            printf("Nome: %s\n",c.username);

            fd_answer = open(str, O_WRONLY);
            if(fd_answer == -1) {
                printf("Erro %d\n", c.PID);
                fflush(stdout);
            }
            else {
                answer = verifyClient(c);
                write(fd_answer, &answer, sizeof(answer));
                close(fd_answer);
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
      
      it = it->p;
    }
    return -1;
  }
}
/* ALTERAR
void *correctingPhrases (void* info) {
    char str[80];
    int fd, fd_answer, answer;
    Client c;

    mkfifo (FIFOLOGIN, 0660);

    fd = open(FIFOLOGIN, O_RDWR);

    while(sair == 0) {
        if((read(fd, &c, sizeof(c))) == sizeof(c)) {
            sprintf(str, "../JJJ%d", c.PID);

            fd_answer = open(str, O_WRONLY);
            if(fd_answer == -1) {
                printf("Erro %d\n", c.PID);
                fflush(stdout);
            }
            else {
                answer = verifyClient(c);
                write(fd_answer, &answer, sizeof(answer));
                close(fd_answer);
            }
        }
    }
}
*/

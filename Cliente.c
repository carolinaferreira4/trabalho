/*
Cliente para o Servidor:
 -> Envia o nome de utilizador  -> nome & pid
 -> Envia a linha -> linha & pid
 -> Envia texto, caso possa escrever -> char frase[nº colunas máximas]
 */

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
  int fd, fp;
  int asw;
  char fifo[50];
  Client c;
  
  sprintf(fifo,FIFOCLI, getpid());
  mkfifo(fifo, 0660);
  
  fd = open(FIFOLOGIN, O_WRONLY);
  if(fd == -1)
  {
    printf("Erro ao abrir fifo");
    exit(1);
  }
  printf("Nome: ");
  scanf("%[^\n]", c.username);
  c.PID = getpid();
  printf("%s - %d", c.username, c.PID);
  
  write(fd, &c, sizeof(c));
  
  fp = open(fifo, O_RDONLY);
  
  read(fp, &asw, sizeof(asw));
  
  if(asw == 1)
  {
      printf("ENTROU");
  } else {
      printf("SAIU");
  }
  
  close(fd);
}



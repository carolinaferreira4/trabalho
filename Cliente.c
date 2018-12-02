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
#include <ncurses.h>

#include "Estruturas.h"


int main(int argv[], int argc){
  int fd, fp;
  char fifo[50];
  Client c;
  Line asw;
  char tecla;
  fd_set fontes;
  int res;
  struct timeval tempo;
  int nLine=0;
    
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
  close(fd);
  
  fp = open(fifo, O_RDONLY);
  
  /// NCURSES
   WINDOW * mainwin;
   
    /*if ((mainwin = initscr()) == NULL) {
        fprintf(stderr, "Error initialising ncurses.\n");
        exit(EXIT_FAILURE);
    }*/
   noecho();
   keypad(mainwin, TRUE);
   curs_set(0);
   start_color();
   
    init_pair(1, COLOR_WHITE, COLOR_BLACK);

  
  do{
      //MONITORIZA O STDIN E O FIFO
	FD_ZERO(&fontes);
	FD_SET(0, &fontes); 	//stdin
	FD_SET(fp, &fontes);	//FIFO
	tempo.tv_sec = 1;	//TIMEOUT
	tempo.tv_usec = 0;

	//BLOQUEIA AQUI ATE MUDAR ALGO NA MONITORIZAÇAO - select()
	res = select(fp + 1, &fontes, NULL, NULL, &tempo);

	if(res == 0) {
	    printf("Nao houve dados ao fim do timeout de um segundo");
	    fflush(stdout);
	} else if(FD_ISSET(0, &fontes)) {	//HA DADOS NO TECLADO
            clear();
            refresh();
            scanf("%c", &tecla);
            if(tecla == 'w'){
                if((nLine-1) >= 0){
                    nLine--;
                }
            } else{
                if(tecla == 's'){
                    nLine++;
                }
            }
              attron(COLOR_PAIR(9));
              mvprintw(60, nLine, "<-");
	} else if(FD_ISSET(fp, &fontes)) { 	//HA DADOS NO FIFO
            read(fp, &asw, sizeof(asw));
            if(asw.c.PID == -2)
            {
                printf("SAIU");
                break;
            } else {
                printf("%s", asw.nLine);
            }
         }
      
  } while(1);
  
    delwin(mainwin);
    endwin();
    refresh();
  close(fp);
}



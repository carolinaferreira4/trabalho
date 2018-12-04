
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define FIFOLOGIN "ssslogin"
#define FIFOREQUEST "sssrequest"
#define FIFOCLI "ccc%d"
    

typedef struct {
    char username [8];
    int PID;
} Client;

typedef struct lC{
    char username [8];
    int PID;
    struct lC* next;
} lClient;

typedef struct {
    int message;
} Message; 

typedef struct {
    char nLine[45];
    Client c;
} Line;

typedef struct{
    char letter;
    int pid;
}Letter;

typedef struct {
    int maxColumns;
    int maxLines;
    Line* fullText;
} Info;


typedef struct {
    int PID;
    int line;
} Request;



#ifdef __cplusplus
}
#endif

#endif /* ESTRUTURAS_H */


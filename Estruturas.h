
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define FIFOLOGIN "ssslogin"
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
    char* nLine;
    Client c;
} Line;

typedef struct {
    int maxColumns;
    int maxLines;
    Line* fullText;
} Info;



#ifdef __cplusplus
}
#endif

#endif /* ESTRUTURAS_H */


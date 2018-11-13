
#ifndef ESTRUTURAS_H
#define ESTRUTURAS_H

#ifdef __cplusplus
extern "C" {
#endif
    
#define FIFOLOGIN "../CCC"

typedef struct {
    char username [8];
    int PID;
} Client;

typedef struct _t{
    char username [8];
    int PID;
    struct _t* p;
} lClient;

typedef struct {
    int* nLine ;
    int PID ;
} Line;

/*
typedef struct {
    char* phrase ;
} Text;
*/

typedef struct {
    int maxColumns;
    int maxLines;
    char** fullText;
} Info;



#ifdef __cplusplus
}
#endif

#endif /* ESTRUTURAS_H */


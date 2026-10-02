#ifndef ESTRUTURA_HISTORICO_H
#define ESTRUTURA_HISTORICO_H

#include <stddef.h> 

//Etrutura da batlha
typedef struct {
    char data[25];        
    char modo[20];      
    char vencedor[50];    
} RegistroBatalha;

void AdicionarRegistroHistorico(RegistroBatalha registro);

#endif
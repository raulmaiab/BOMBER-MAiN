#ifndef MENU_PROXIMO_H
#define MENU_PROXIMO_H

#include "raylib.h"
#include "options.h" 

//Estrutura da função
typedef struct {
    int nivelAtual;         
    const char* proximoMapa; 
} InfoNivel;

AcaoOpcoes ExecutarMenuProximoNivel(InfoNivel info);

#endif
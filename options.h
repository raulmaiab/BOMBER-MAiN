#ifndef OPCOES_H
#define OPCOES_H

#include "raylib.h"
#include <stdbool.h>

//Menu
typedef enum {
    OPCOES_ACAO_RESUMO,    
    OPCOES_ACAO_REINICIAR,
    OPCOES_ACAO_EDITAR_JOGO, 
    OPCOES_ACAO_MENU_PRINCIPAL,
    OPCOES_ACAO_NENHUMA     
} AcaoOpcoes;

AcaoOpcoes ExecutarMenuOpcoes(void);

#endif
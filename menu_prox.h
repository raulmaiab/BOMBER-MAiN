// menu_prox.h

#ifndef MENU_PROX_H
#define MENU_PROX_H

#include "raylib.h"
#include "options.h" // Para usar OptionsAction

// Definição da estrutura para passar informações ao menu de progresso
typedef struct {
    int nivelAtual;         // Índice do nível que acaba de ser concluído (0, 1, 2)
    const char* proximoMapa; // Nome do mapa do próximo nível
} NivelInfo;

// Função que executa a tela de transição de nível
// Retorna a ação escolhida pelo jogador (START ou EXIT)
OptionsAction ExecutarMenuProximoNivel(NivelInfo info);

#endif // MENU_PROX_H
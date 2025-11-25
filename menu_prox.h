// menu_prox.h

#ifndef MENU_PROXIMO_H
#define MENU_PROXIMO_H

#include "raylib.h"
#include "options.h" // Para usar AcaoOpcoes

// Definição da estrutura para passar informações ao menu de progresso
typedef struct {
    int nivelAtual;         // Índice do nível que acaba de ser concluído (0, 1, 2)
    const char* proximoMapa; // Nome do mapa do próximo nível
} InfoNivel;

/**
 * @brief Função que executa a tela de transição de nível após uma vitória.
 *
 * @param info Estrutura contendo o nível atual e o nome do próximo mapa.
 * @return Retorna a ação escolhida pelo jogador. Retorna OPCOES_ACAO_NENHUMA para continuar 
 * ou OPCOES_ACAO_MENU_PRINCIPAL para sair.
 */
AcaoOpcoes ExecutarMenuProximoNivel(InfoNivel info);

#endif // MENU_PROXIMO_H
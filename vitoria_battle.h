#ifndef VITORIA_BATTLE_H
#define VITORIA_BATTLE_H

#include <stdbool.h> 

/**
 * @brief Executa a tela de "You Won" para o modo Batalha.
 * @param winnerColor O nome da cor/sprite do jogador vencedor (ex: "Branco", "Preto").
 * @return Retorna 'true' se o jogador pressionar ENTER (para voltar ao menu), 
 * ou 'false' se fechar a janela ou pressionar ESC.
 */
void ExecutarTelaVitoriaBattle(const char* winnerColor);

#endif // VITORIA_BATTLE_H
#ifndef TELA_VITORIA_BATALHA_H
#define TELA_VITORIA_BATALHA_H

#include <stdbool.h> 

/**
 * @brief Executa a tela de "Você Venceu" para o modo Batalha (BATTLE).
 * @param nomeSpriteVencedor O nome da cor/sprite do jogador vencedor (ex: "SpriteBranco").
 * @return void (A função apenas executa a tela e retorna ao loop principal ao receber input).
 */
void ExecutarTelaVitoriaBattle(const char* nomeSpriteVencedor);

#endif // TELA_VITORIA_BATALHA_H
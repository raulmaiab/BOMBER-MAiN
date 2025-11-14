#ifndef MENU_BATTLE_H
#define MENU_BATTLE_H

#include <stdbool.h>

/**
 * @brief Armazena as configurações escolhidas pelo jogador
 * para a partida Battle.
 */
typedef struct {
    int numPlayers; // 1 ou 2
    bool extras;    // false = Off, true = On
    int mapIndex;   // 0 = Mapa 1, 1 = Mapa 2, etc.
} BattleSettings;

/**
 * @brief Executa a tela de configuração da Batalha.
 * @param settings Um ponteiro para a struct que será preenchida com as escolhas.
 * @return Retorna 'true' se o jogador confirmar (ENTER), 
 * ou 'false' se cancelar (ESC).
 */
bool ExecutarMenuBattle(BattleSettings *settings);

#endif // MENU_BATTLE_H
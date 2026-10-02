#ifndef MENU_BATTLE_H
#define MENU_BATTLE_H

#include <stdbool.h>


typedef struct {
    int numero_jogadores; //1 ou 2
    bool extras_habilitados;    //Ligado ou desligado
    int indice_mapa;   //Índice do mapa
} BattleSettings;


bool ExecutarMenuBattle(BattleSettings *configuracoes);

#endif
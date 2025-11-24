#ifndef MENU_BATTLE_H
#define MENU_BATTLE_H

#include <stdbool.h>


typedef struct {
    int numero_jogadores; //1 ou 2
    bool extras_habilitados;    //false = Desligado, true = Ligado
    int indice_mapa;   //0 = Mapa 1 (Default), 1 = Mapa 2 (Cave), etc.
} BattleSettings;


bool ExecutarMenuBattle(BattleSettings *configuracoes);

#endif
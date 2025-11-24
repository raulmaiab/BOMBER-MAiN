#ifndef MENU_STORY_H
#define MENU_STORY_H

#include <stdbool.h>

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} StoryDifficulty;

typedef struct {
    //CORREÇÃO: Traduzido para português para corresponder aos arquivos .c
    int numero_jogadores;     //1 ou 2
    bool extras_habilitados;  //true/false
    StoryDifficulty difficulty;
} StorySettings;

// Retorna true se o jogador selecionou START, false se apertou ESC
bool ExecutarMenuStory(StorySettings *settings);

#endif 
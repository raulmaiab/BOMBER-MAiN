#ifndef MENU_STORY_H
#define MENU_STORY_H

#include <stdbool.h>

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} StoryDifficulty;

typedef struct {
    int numero_jogadores;    
    bool extras_habilitados; 
    StoryDifficulty difficulty;
} StorySettings;

//Retorna true se o jogador selecionou START, false se apertou ESC
bool ExecutarMenuStory(StorySettings *settings);

#endif 
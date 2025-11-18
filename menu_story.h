#ifndef MENU_STORY_H
#define MENU_STORY_H

#include <stdbool.h>

typedef enum {
    DIFFICULTY_EASY,
    DIFFICULTY_MEDIUM,
    DIFFICULTY_HARD
} StoryDifficulty;

typedef struct {
    int numPlayers;     // 1 ou 2
    bool extras;        // true/false
    StoryDifficulty difficulty;
} StorySettings;

// Retorna true se o jogador escolheu START, false se apertou ESC
bool ExecutarMenuStory(StorySettings *settings);

#endif // MENU_STORY_H
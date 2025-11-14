#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>

// --- Cores personalizadas (Sem alteração) ---
#define COLOR_UI_BACKGROUND CLITERAL(Color){ 20, 30, 40, 255 }   
#define COLOR_UI_TOP_BAR    CLITERAL(Color){ 30, 40, 50, 200 }   
#define COLOR_BLUE_HIGHLIGHT CLITERAL(Color){ 0, 150, 200, 255 } 
#define COLOR_GRAY_OPTION   CLITERAL(Color){ 100, 100, 100, 255 } 
#define COLOR_YELLOW_HIGHLIGHT CLITERAL(Color){ 255, 200, 0, 255 } 

// Enum de Opções (Sem alteração)
typedef enum {
    ESCOLHA_BATTLE,
    ESCOLHA_STORY,
    ESCOLHA_SHOP,
    ESCOLHA_OTHER,
    ESCOLHA_SAIR,
    ESCOLHA_NENHUMA_OU_FECHOU 
} OpcaoMenu;

// --- Estrutura dos Itens do Menu (Sem alteração) ---
typedef struct {
    const char* texto;
    OpcaoMenu idOpcao;
} MenuItem;

// --- Estruturas para os Efeitos de Fundo (Sem alteração) ---
#define MAX_RAIN_DROPS 100
#define MAX_PULSES 10
typedef struct { Vector2 pos; float speed; } RainDrop;
typedef struct { Vector2 center; float radius; float alpha; } Pulse;

// --- ATUALIZADO: Variável movida para o .c ---
// static bool effectsInitialized = false; // <-- LINHA REMOVIDA

// --- Funções (internas, não precisam estar no .h) ---
// static void InitMenuEffects(void);
// static void UpdateMenuEffects(void);
// static void DrawMenuEffects(void);

// Função principal (A única que o bombermain.c precisa ver)
OpcaoMenu ExecutarTelaMenu(void);

#endif // MENU_H
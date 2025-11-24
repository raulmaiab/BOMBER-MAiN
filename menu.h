#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <stdbool.h>

//cores personalizadas
#define COLOR_UI_BACKGROUND CLITERAL(Color){ 20, 30, 40, 255 }   
#define COLOR_UI_TOP_BAR    CLITERAL(Color){ 30, 40, 50, 200 }   
#define COLOR_BLUE_HIGHLIGHT CLITERAL(Color){ 0, 150, 200, 255 } 
#define COLOR_GRAY_OPTION   CLITERAL(Color){ 100, 100, 100, 255 } 
#define COLOR_YELLOW_HIGHLIGHT CLITERAL(Color){ 255, 200, 0, 255 } 

//Enumerando as escolhas do menu
typedef enum {
    ESCOLHA_BATTLE,
    ESCOLHA_STORY,
    ESCOLHA_SAIR,
    ESCOLHA_NENHUMA_OU_FECHOU 
} OpcaoMenu;

//itens do menu
typedef struct {
    const char* texto;
    OpcaoMenu idOpcao;
} MenuItem;

//animacoes dos efeitos
#define MAX_GOTAS_CHUVA 100
#define MAX_PULSOS_CIRCULARES 10

//Gota
typedef struct { 
    Vector2 posicao; 
    float velocidade; 
} RainDrop;

//Onda de choque
typedef struct { 
    Vector2 centro; 
    float raio; 
    float alpha; 
} Pulse;

//def principal
OpcaoMenu ExecutarTelaMenu(void);

#endif
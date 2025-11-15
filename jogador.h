#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" 

// --- Constantes (Sem alteração) ---
typedef enum { BOT_STATE_WANDERING, BOT_STATE_FLEEING, BOT_STATE_HOLDING } BotState;
#define NUM_FRAMES_ANDAR 3 
#define ANIM_FRAME_SPEED 0.15f 
typedef enum { DIR_CIMA=0, DIR_BAIXO, DIR_ESQUERDA, DIR_DIREITA, DIR_PARADO } AnimDirection;

// --- Struct Jogador (Sem alteração) ---
typedef struct Jogador
{
    Vector2 pos;       
    float velocidade;  
    Texture2D texParado; 
    Texture2D texCima[NUM_FRAMES_ANDAR];
    Texture2D texBaixo[NUM_FRAMES_ANDAR];
    Texture2D texEsquerda[NUM_FRAMES_ANDAR];
    Texture2D texDireita[NUM_FRAMES_ANDAR];
    int currentFrame;           
    float frameTimer;           
    AnimDirection currentDir;   
    bool vivo;         
    bool ehBot;
    BotState botState;      
    float botStateTimer;    
    int botMoveDirecao;     
    Vector2 botLastBombPos; 
    float bombaCooldown; 
} Jogador;

// Assinatura CriarJogador (Sem alteração)
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot);

// --- ATUALIZADO: Assinatura da Função ---
// Adiciona os dois alvos humanos (targetHuman2 pode ser NULL)
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, 
                      NodeBombas *gBombas, float deltaTime, 
                      Jogador* targetHuman1, Jogador* targetHuman2);
// --- FIM DA ATUALIZAÇÃO ---

void DesenharJogador(const Jogador* j);
void DestruirJogador(Jogador* j);

#endif // JOGADOR_H
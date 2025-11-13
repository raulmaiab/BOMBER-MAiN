#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" 

// --- Estados da IA --- (Sem alteração)
typedef enum {
    BOT_STATE_WANDERING, 
    BOT_STATE_FLEEING    
} BotState;

// --- NOVO: Constantes de Animação ---
#define NUM_FRAMES_ANDAR 3 // Para costas1, costas2, costas3, etc.
#define ANIM_FRAME_SPEED 0.15f // Velocidade da animação (tempo por frame)

// --- NOVO: Direção de Animação ---
typedef enum {
    DIR_CIMA = 0,
    DIR_BAIXO,
    DIR_ESQUERDA,
    DIR_DIREITA,
    DIR_PARADO // Quando o jogador não se move
} AnimDirection;
// --- FIM NOVO ---


typedef struct Jogador
{
    Vector2 pos;       
    float velocidade;  
    
    // --- ATUALIZADO: Texturas para Animação ---
    Texture2D texParado; // O frame 'andando' ou 'costas' padrão
    Texture2D texCima[NUM_FRAMES_ANDAR];
    Texture2D texBaixo[NUM_FRAMES_ANDAR];
    Texture2D texEsquerda[NUM_FRAMES_ANDAR];
    Texture2D texDireita[NUM_FRAMES_ANDAR];

    int currentFrame;           // Índice do frame atual (0, 1, 2)
    float frameTimer;           // Temporizador para mudar de frame
    AnimDirection currentDir;   // Direção atual para selecionar a textura
    // --- FIM DA ATUALIZAÇÃO ---

    bool vivo;         
    
    // --- Variáveis do Bot (Sem alteração) ---
    bool ehBot;
    BotState botState;      
    float botStateTimer;    
    int botMoveDirecao;     
    
} Jogador;

// --- ATUALIZADO: CriarJogador (novo parâmetro para a pasta) ---
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot);

// Assinatura de AtualizarJogador (Sem alteração)
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas, float deltaTime);

void DesenharJogador(const Jogador* j);

// --- ATUALIZADO: DestruirJogador (para descarregar todas as texturas) ---
void DestruirJogador(Jogador* j);

#endif // JOGADOR_H
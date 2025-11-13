#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" // Incluído

// --- NOVO: Estados da IA ---
typedef enum {
    BOT_STATE_WANDERING, // Passeando
    BOT_STATE_FLEEING    // Fugindo
} BotState;
// --- FIM NOVO ---

typedef struct Jogador
{
    Vector2 pos;       
    float velocidade;  
    Texture2D textura; 
    bool vivo;         
    
    // --- Variáveis do Bot ATUALIZADAS ---
    bool ehBot;
    BotState botState;      // O estado atual (WANDERING ou FLEEING)
    float botStateTimer;    // Temporizador para o estado atual
    int botMoveDirecao;     // 0=Cima, 1=Baixo, 2=Esquerda, 3=Direita
    // --- FIM DA ATUALIZAÇÃO ---
    
} Jogador;

// Assinatura ATUALIZADA (só mudou 'ehBot')
Jogador CriarJogador(Vector2 posInicial, const char* imgSprite, bool ehBot);

// Assinatura ATUALIZADA (passa deltaTime)
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas, float deltaTime);

void DesenharJogador(const Jogador* j);

void DestruirJogador(Jogador* j);

#endif
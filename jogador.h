#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" 

// --- ATUALIZADO: Estados da IA ---
typedef enum {
    BOT_STATE_WANDERING, // Passeando
    BOT_STATE_FLEEING,   // Fugindo (para sair do tile da bomba)
    BOT_STATE_HOLDING    // Parado (esperando a bomba explodir)
} BotState;
// --- FIM DA ATUALIZAÇÃO ---

// --- Constantes (Sem alteração) ---
#define NUM_FRAMES_ANDAR 3 
#define ANIM_FRAME_SPEED 0.15f 
typedef enum { DIR_CIMA=0, DIR_BAIXO, DIR_ESQUERDA, DIR_DIREITA, DIR_PARADO } AnimDirection;

typedef struct Jogador
{
    Vector2 pos;       
    float velocidade;  
    
    // Texturas de Animação (Sem alteração)
    Texture2D texParado; 
    Texture2D texCima[NUM_FRAMES_ANDAR];
    Texture2D texBaixo[NUM_FRAMES_ANDAR];
    Texture2D texEsquerda[NUM_FRAMES_ANDAR];
    Texture2D texDireita[NUM_FRAMES_ANDAR];
    int currentFrame;           
    float frameTimer;           
    AnimDirection currentDir;   

    bool vivo;         
    
    // --- Variáveis do Bot ATUALIZADAS ---
    bool ehBot;
    BotState botState;      
    float botStateTimer;    // Temporizador para o estado atual
    int botMoveDirecao;     // 0-3 (Move), 4 (Parado)
    Vector2 botLastBombPos; // Onde o bot plantou a bomba
    // --- FIM DA ATUALIZAÇÃO ---
    
    float bombaCooldown; 
    
} Jogador;

// Assinaturas (Sem alteração)
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot);
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, NodeBombas *gBombas, float deltaTime);
void DesenharJogador(const Jogador* j);
void DestruirJogador(Jogador* j);

#endif // JOGADOR_H
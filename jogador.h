#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" 

// --- Estados da IA ---
typedef enum {
    BOT_STATE_WANDERING, 
    BOT_STATE_FLEEING,   
    BOT_STATE_HOLDING    
} BotState;

// --- Constantes ---
#define NUM_FRAMES_ANDAR 3 
#define ANIM_FRAME_SPEED 0.15f 
#define MAX_SPRITE_NAME_LENGTH 16 // <<< NOVO: Definição do tamanho para o nome/cor
typedef enum { DIR_CIMA=0, DIR_BAIXO, DIR_ESQUERDA, DIR_DIREITA, DIR_PARADO } AnimDirection;

typedef struct Jogador
{
    Vector2 pos;       
    float velocidade;  
    
    // Texturas
    Texture2D texParado; 
    Texture2D texCima[NUM_FRAMES_ANDAR];
    Texture2D texBaixo[NUM_FRAMES_ANDAR];
    Texture2D texEsquerda[NUM_FRAMES_ANDAR];
    Texture2D texDireita[NUM_FRAMES_ANDAR];
    int currentFrame;           
    float frameTimer;           
    AnimDirection currentDir;   

    bool vivo;         
    
    char spriteName[MAX_SPRITE_NAME_LENGTH]; // <<< CAMPO CORRIGIDO: Nome do sprite/cor
    
    // Variáveis do Bot
    bool ehBot;
    BotState botState;      
    float botStateTimer;    
    int botMoveDirecao;     
    Vector2 botLastBombPos; 
    
    float bombaCooldown; 
    
    //Atributos de Power-ups (Extras) ---
    int bombRange;          // Alcance da bomba (Começa em 1)
    
    bool temDefesa;         // Se tem escudo
    float timerDefesa;      // Tempo restante do escudo
    
    bool temVelocidade;     // Se tem speed boost
    float timerVelocidade;  // Tempo restante da velocidade

    int bombLimit;          // Limite de Bombas Simultaneas
    int bombasAtivas;       // Bombas Ativas
    // ---------------------------------------------
    
} Jogador;

// Assinaturas
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot);

// Atualizar recebe os alvos para a IA
void AtualizarJogador(Jogador* j, int keyUp, int keyDown, int keyLeft, int keyRight, int keyBomb, 
                      NodeBombas *gBombas, float deltaTime, 
                      Jogador* targetHuman1, Jogador* targetHuman2);

void DesenharJogador(const Jogador* j);
void DestruirJogador(Jogador* j);

#endif // JOGADOR_H
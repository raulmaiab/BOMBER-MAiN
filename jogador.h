#ifndef JOGADOR_H
#define JOGADOR_H

#include "raylib.h"
#include <stdbool.h>
#include "bomba.h" 

// --- Estados da IA ---
typedef enum {
    BOT_ESTADO_VAGANDO,    // BOT_STATE_WANDERING
    BOT_ESTADO_FUGINDO,    // BOT_STATE_FLEEING
    BOT_ESTADO_ESPERANDO   // BOT_STATE_HOLDING
} EstadoBot;

// --- Constantes ---
#define NUM_FRAMES_ANDAR 3 
#define VELOCIDADE_FRAME_ANIMACAO 0.15f 
#define COMPRIMENTO_MAX_NOME_SPRITE 16 

typedef enum { DIR_CIMA=0, DIR_BAIXO, DIR_ESQUERDA, DIR_DIREITA, DIR_PARADO } DirecaoAnimacao;

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
    int frameAtual;           
    float temporizadorFrame;           
    DirecaoAnimacao direcaoAtual;   

    bool vivo;         
    
    char nomeSprite[COMPRIMENTO_MAX_NOME_SPRITE]; 
    
    // Variáveis do Bot
    bool ehBot;
    EstadoBot estadoBot;      
    float temporizadorEstadoBot;    
    int direcaoMovimentoBot;     
    Vector2 ultimaPosicaoBombaBot; 
    
    float recargaBomba; 
    
    //Atributos de Power-ups (Extras) ---
    int alcanceBomba;          
    
    bool temDefesa;         
    float temporizadorDefesa;      
    
    bool temVelocidade;     
    float temporizadorVelocidade;  

    int limiteBombas;          
    int bombasAtivas;       
    // ---------------------------------------------
    
} Jogador;

// Assinaturas (Traduzidas)
Jogador CriarJogador(Vector2 posInicial, const char* pastaSprites, bool ehBot);

// Atualizar recebe os alvos para a IA
void AtualizarJogador(Jogador* j, int teclaCima, int teclaBaixo, int teclaEsquerda, int teclaDireita, int teclaBomba, 
                      NodeBombas *gBombas, float deltaTime, 
                      Jogador* alvoHumano1, Jogador* alvoHumano2);

void DesenharJogador(const Jogador* j);
void DestruirJogador(Jogador* j);

#endif // JOGADOR_H
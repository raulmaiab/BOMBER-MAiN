#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" 

#define MAX_BOMBAS_ATIVAS 20

// Forward Declaration: Diz ao compilador que existe uma struct Jogador
// Isso é necessário antes de includer o "jogador.h"
struct Jogador;

typedef struct Bomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao;         
    bool ativa;               
    int frameAtual;         
    float temporizadorFrame;
    struct Jogador *dono; // Ponteiro para o dono da bomba
    
} Bomba;

typedef struct {
    Bomba bombas[MAX_BOMBAS_ATIVAS]; 
    int quantidade;                  
    Texture2D texNormal; 
    Texture2D texAviso;  
} NodeBombas;

NodeBombas CriarNodeBombas(void); 

// Assinatura corrigida para aceitar ponteiro para Jogador
void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, struct Jogador *dono);

bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores);

void DesenharBombas(const NodeBombas *g);

void UnloadBombas(NodeBombas *g);

#endif // BOMBA_H
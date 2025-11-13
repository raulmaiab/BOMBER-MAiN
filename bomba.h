#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" 

#define MAX_BOMBAS_ATIVAS 20

// --- NOVO: Forward Declaration ---
// Dizemos ao compilador "existe uma struct Jogador",
// para evitar erros de inclusão circular com jogador.h
struct Jogador;
// --- FIM NOVO ---

typedef struct Bomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao;         
    bool ativa;               
    
    int currentFrame;         
    float frameTimer;         
    
} Bomba;

typedef struct {
    Bomba bombas[MAX_BOMBAS_ATIVAS]; 
    int quantidade;                  
    Texture2D texNormal; 
    Texture2D texAviso;  
} NodeBombas;

NodeBombas CriarNodeBombas(void); 

void PlantarBomba(NodeBombas *g, Vector2 posBomba);

// --- ATUALIZADO: Assinatura da função ---
// Agora aceita um array de ponteiros de Jogador e a quantidade.
bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores);
// --- FIM DA ATUALIZAÇÃO ---

void DesenharBombas(const NodeBombas *g);

void UnloadBombas(NodeBombas *g);

#endif // BOMBA_H
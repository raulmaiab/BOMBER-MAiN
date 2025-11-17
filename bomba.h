#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" 

#define MAX_BOMBAS_ATIVAS 20

// Forward Declaration para evitar inclusão circular
struct Jogador;

typedef struct Bomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao; // Este valor agora será dinâmico        
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

// --- ATUALIZADO: Agora aceita o 'range' do jogador ---
void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range);
// --- FIM DA ATUALIZAÇÃO ---

bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores);

void DesenharBombas(const NodeBombas *g);

void UnloadBombas(NodeBombas *g);

#endif // BOMBA_H
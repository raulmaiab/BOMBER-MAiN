#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" 

#define MAX_BOMBAS_ATIVAS 20

// Forward Declaration ---
// Dizemos ao compilador "existe uma struct Jogador",
// para evitar erros de inclusão circular com jogador.h
struct Jogador;


typedef struct Bomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao; // Este valor agora será dinâmico        
    bool ativa;               
    int currentFrame;         
    float frameTimer;
    struct Jogador *dono;
    
} Bomba;

typedef struct {
    Bomba bombas[MAX_BOMBAS_ATIVAS]; 
    int quantidade;                  
    Texture2D texNormal; 
    Texture2D texAviso;  
} NodeBombas;

NodeBombas CriarNodeBombas(void); 

void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, Jogador *dono);

bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores);

void DesenharBombas(const NodeBombas *g);

void UnloadBombas(NodeBombas *g);

#endif // BOMBA_H
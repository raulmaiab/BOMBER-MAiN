#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" 

struct Jogador;

typedef struct NodeBomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao;         
    bool ativa;               
    int frameAtual;         
    float temporizadorFrame;
    struct Jogador *dono; 
    struct NodeBomba *next; 
    
} NodeBomba;

typedef struct {
    NodeBomba *head; 
    int quantidade;  
    Texture2D texNormal; 
    Texture2D texAviso;  
} NodeBombas;

NodeBombas CriarNodeBombas(void); 

void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, struct Jogador *dono);

bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores);

void DesenharBombas(const NodeBombas *g);

void UnloadBombas(NodeBombas *g);

#endif
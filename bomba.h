#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>
#include "explosao.h" 

// MAX_BOMBAS_ATIVAS não é mais necessário, mas pode ser mantido para um limite suave.

// Forward Declaration:
struct Jogador;

// ----------------------------------------------------------------------
// ESTRUTURA DO NÓ DA BOMBA (Substitui a struct Bomba original)
// ----------------------------------------------------------------------
typedef struct NodeBomba 
{
    Vector2 posicao;          
    float tempoExplosao;      
    int raioExplosao;         
    bool ativa;               
    int frameAtual;         
    float temporizadorFrame;
    struct Jogador *dono; // Ponteiro para o dono da bomba
    
    // NOVO: Ponteiro para o próximo nó da lista encadeada
    struct NodeBomba *next; 
    
} NodeBomba;

// ----------------------------------------------------------------------
// ESTRUTURA DA LISTA (Apenas o cabeçalho)
// ----------------------------------------------------------------------
typedef struct {
    NodeBomba *head; // Ponteiro para o primeiro nó
    int quantidade;  // Contagem atual de bombas (opcional, mas útil)
    Texture2D texNormal; 
    Texture2D texAviso;  
} NodeBombas;

NodeBombas CriarNodeBombas(void); 

// Assinatura de PlantarBomba para ADICIONAR um novo nó (alocação dinâmica)
void PlantarBomba(NodeBombas *g, Vector2 posBomba, int range, struct Jogador *dono);

// Assinatura de AtualizarBombas para ITERAR, gerenciar o tempo, e REMOVER nós
bool AtualizarBombas(NodeBombas *g, float deltaTime, NodeExplosoes *gExplosoes, struct Jogador* jogadores[], int numJogadores);

// Assinatura de DesenharBombas para ITERAR sobre os nós
void DesenharBombas(const NodeBombas *g);

// Assinatura de UnloadBombas para DESTRUIR a lista e liberar a memória
void UnloadBombas(NodeBombas *g);

#endif // BOMBA_H
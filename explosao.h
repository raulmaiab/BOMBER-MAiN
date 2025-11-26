#ifndef EXPLOSAO_H
#define EXPLOSAO_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_EXPLOSOES 100 

struct Jogador; 

typedef struct {
    Vector2 pos;
    bool ativa;
    int frameAtual;
    float temporizadorFrame;
} Explosao;

typedef struct {
    Explosao explosoes[MAX_EXPLOSOES];
    int quantidade; 
    Texture2D texExplo1;
    Texture2D texExplo2;
} NodeExplosoes;

NodeExplosoes CriarNodeExplosoes(void);

void AtivarExplosao(NodeExplosoes *g, Vector2 pos);

void CriarExplosao(NodeExplosoes *g, Vector2 centro, int alcance, struct Jogador* jogadores[], int numJogadores);

void AtualizarExplosoes(NodeExplosoes *g, float deltaTime);
void DesenharExplosoes(const NodeExplosoes *g);
void DescarregarExplosoes(NodeExplosoes *g);

#endif
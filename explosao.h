#ifndef EXPLOSAO_H
#define EXPLOSAO_H

#include "raylib.h"
#include <stdbool.h>

#define MAX_EXPLOSOES 100 // Aumentei para garantir que cabem todas as chamas

// Forward declaration para evitar erros de inclusão cruzada
struct Jogador; 

typedef struct {
    Vector2 pos;
    bool ativa;
    int frameAtual;
    float temporizadorFrame;
} Explosao;

typedef struct {
    Explosao explosoes[MAX_EXPLOSOES];
    int quantidade; // Usado para controle de slots (e não necessariamente o número exato de ativos)
    Texture2D texExplo1;
    Texture2D texExplo2;
} NodeExplosoes;

NodeExplosoes CriarNodeExplosoes(void);

// Apenas liga o visual (uso interno)
void AtivarExplosao(NodeExplosoes *g, Vector2 pos);

// --- NOVA FUNÇÃO PRINCIPAL ---
// Gerencia a lógica: quebra blocos, spawna extras e mata jogadores
void CriarExplosao(NodeExplosoes *g, Vector2 centro, int alcance, struct Jogador* jogadores[], int numJogadores);
// -----------------------------

void AtualizarExplosoes(NodeExplosoes *g, float deltaTime);
void DesenharExplosoes(const NodeExplosoes *g);
void DescarregarExplosoes(NodeExplosoes *g);

#endif // EXPLOSAO_H
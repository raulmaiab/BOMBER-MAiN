#include "bomba.h"
#include "raylib.h"

NodeBombas CriarNodeBombas(const char *sprite) {
    NodeBombas g;
    g.quantidade = 0;
    g.sprite = LoadTexture(sprite);
    return g;
}

void PlantarBomba(NodeBombas *g, Vector2 jogadorPos, Vector2 direcao, int tamanhoBloco) {
    if (g->quantidade >= 20) return;

    // Posição 1 bloco à frente 
    Vector2 pos = {
        jogadorPos.x + direcao.x * tamanhoBloco,
        jogadorPos.y + direcao.y * tamanhoBloco
    };

    Bomba *b = &g->bombas[g->quantidade];

    b->posicao = pos;
    b->tempoExplosao = 2.5f;   // sempre explode após 2.5s
    b->raioExplosao = 2;
    b->sprite = g->sprite;

    g->quantidade++;
}

bool AtualizarBombas(NodeBombas *g, float deltaTime) {
    bool algumaExplodiu = false;

    for (int i = 0; i < g->quantidade; i++) {
        Bomba *b = &g->bombas[i];

        // Contagem regressiva
        b->tempoExplosao -= deltaTime;

        // Explodiu
        if (b->tempoExplosao <= 0) {

            algumaExplodiu = true;

            // Remove a bomba substituindo pelo último elemento
            g->bombas[i] = g->bombas[g->quantidade - 1];

            g->quantidade--;
            i--; // revisar o índice atual
        }
    }

    return algumaExplodiu;
}

void DesenharBombas(const NodeBombas *g) {
    for (int i = 0; i < g->quantidade; i++) {
        const Bomba *b = &g->bombas[i];
        DrawTexture(b->sprite, b->posicao.x, b->posicao.y, WHITE);
    }
}

void UnloadBombas(NodeBombas *g) {
    UnloadTexture(g->sprite);
}

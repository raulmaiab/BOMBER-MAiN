#ifndef EXPLOSAO_H
#define EXPLOSAO_H

#include "raylib.h"
#include <stdbool.h>

// Quantas explosões podem estar na tela ao mesmo tempo
#define MAX_EXPLOSOES 50

typedef struct {
    Vector2 pos;        // Posição (em pixels)
    bool ativa;         // Está ativa?
    float frameTimer;   // Temporizador da animação
    int currentFrame;   // Frame atual (0 = explo1.png, 1 = explo2.png)
} Explosao;

typedef struct {
    Explosao explosoes[MAX_EXPLOSOES];
    int quantidade;
    Texture2D texExplo1; // Textura para explo1.png
    Texture2D texExplo2; // Textura para explo2.png
} NodeExplosoes;

/**
 * @brief Carrega as texturas da explosão (explo1.png, explo2.png)
 */
NodeExplosoes CriarNodeExplosoes(void);

/**
 * @brief Ativa uma nova animação de explosão na posição dada.
 */
void AtivarExplosao(NodeExplosoes *g, Vector2 pos);

/**
 * @brief Atualiza o timer de todas as explosões ativas.
 */
void AtualizarExplosoes(NodeExplosoes *g, float deltaTime);

/**
 * @brief Desenha todas as explosões ativas.
 */
void DesenharExplosoes(const NodeExplosoes *g);

/**
 * @brief Descarrega as texturas da explosão.
 */
void UnloadExplosoes(NodeExplosoes *g);

#endif // EXPLOSAO_H
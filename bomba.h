#ifndef BOMBA_H
#define BOMBA_H

#include "raylib.h"
#include <stdbool.h>

typedef struct Bomba 
{
    Vector2 posicao;
    float tempoExplosao;      // quanto tempo a bomba leva para explodir
    int raioExplosao;        // tamanho da explosão em blocos
    Texture2D sprite;          //desenho da bomba
} Bomba;

typedef struct {
    Bomba *head;
    int quantidade;
    Texture2D sprite;
} NodeBombas;

NodeBombas CriarNodeBombas(const char *sprite);

// Cria uma bomba desativada (não aparece ainda)
Bomba CriarBomba(const char *renderSprite);

// Posiciona e ativa a bomba
void PlantarBomba(Bomba *bomba, Vector2 posicao);

// Atualiza contagem do tempo da bomba
bool AtualizarBomba(Bomba *bomba, float deltaTime);

// Desenha a bomba se estiver ativa
void DesenharBomba(const Bomba *bomba);

// Limpa texturas
void UnloadBomba(Bomba *bomba);

#endif

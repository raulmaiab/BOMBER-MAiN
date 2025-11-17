#ifndef EXTRAS_H
#define EXTRAS_H

#include "raylib.h"
#include "jogador.h" // Precisa conhecer o Jogador para modificar seus status

typedef enum {
    EXTRA_NONE,
    EXTRA_RANGE,    // Aumenta explosão (Permanente)
    EXTRA_DEFENSE,  // Escudo (Tempo limitado)
    EXTRA_SPEED     // Velocidade (Tempo limitado)
} ExtraType;

typedef struct {
    Vector2 pos;
    ExtraType type;
    bool ativo;
} ExtraItem;

// Configuração máxima de extras simultâneos no mapa
#define MAX_EXTRAS 20 

void InicializarExtras(void);
void SpawnarExtra(Vector2 gridPos); // Tenta criar um extra numa posição (chance aleatória)
void DesenharExtras(void);
void VerificarColetaExtras(Jogador* j); // Verifica se o jogador pegou algo
void DescarregarExtras(void);

#endif // EXTRAS_H
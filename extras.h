#ifndef EXTRAS_H
#define EXTRAS_H

#include "raylib.h"
#include "jogador.h" 

typedef enum {
    EXTRA_RANGE,    // 0
    EXTRA_DEFENSE,  // 1
    EXTRA_SPEED,    // 2
    EXTRA_MAX       // 3
} ExtraType;

typedef struct {
    Vector2 pos;
    ExtraType type;
    bool ativo;
} ExtraItem; 

#define MAX_EXTRAS 20 

void InicializarExtras(void);
void SpawnarExtra(Vector2 pixelPos); 
void DesenharExtras(void);
void VerificarColetaExtras(Jogador* j); 
void DescarregarExtras(void);

// --- NOVO: Função para a IA encontrar itens ---
// Retorna true se achou algum item dentro do raio.
// Preenche 'posOut' com a posição do item mais próximo.
bool GetExtraMaisProximo(Vector2 posJogador, float raioBusca, Vector2* posOut);
// ----------------------------------------------

#endif // EXTRAS_H
#ifndef EXTRAS_H
#define EXTRAS_H

#include "raylib.h"
#include "jogador.h" 

typedef enum {
    EXTRA_RANGE,      // 0
    EXTRA_DEFENSE,    // 1
    EXTRA_SPEED,      // 2
    EXTRA_BOMB_LIMIT, // 3 <--- O NOVO TIPO TEM QUE ESTAR AQUI
    EXTRA_MAX         // 4 (Total para o Random)
} ExtraType;

typedef struct {
    Vector2 pos;
    ExtraType type;
    bool ativo;
} ExtraItem; 

#define MAX_EXTRAS 20 

void InicializarExtras(void);
void SetExtrasHabilitados(bool habilitado);
void SpawnarExtra(Vector2 pixelPos); 
void DesenharExtras(void);
void VerificarColetaExtras(Jogador* j); 
void DescarregarExtras(void);
void ResetarExtras(void);

bool GetExtraMaisProximo(Vector2 posJogador, float raioBusca, Vector2* posOut);

#endif // EXTRAS_H
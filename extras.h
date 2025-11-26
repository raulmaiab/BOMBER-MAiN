#ifndef EXTRAS_H
#define EXTRAS_H

#include "raylib.h"
#include "jogador.h" 

typedef enum {
    EXTRA_ALCANCE,      
    EXTRA_DEFESA,       
    EXTRA_VELOCIDADE,   
    EXTRA_LIMITE_BOMBA, 
    EXTRA_MAX           
} TipoExtra;

typedef struct {
    Vector2 pos;
    TipoExtra tipo;
    bool ativo;
} ItemExtra; 

#define MAX_EXTRAS 20 

void InicializarExtras(void);
void DefinirExtrasHabilitados(bool habilitado);
void SpawnarExtra(Vector2 posPixel); 
void DesenharExtras(void);
void VerificarColetaExtras(Jogador* j); 
void DescarregarExtras(void);
void ResetarExtras(void);

bool ObterExtraMaisProximo(Vector2 posJogador, float raioBusca, Vector2* posOut);

#endif
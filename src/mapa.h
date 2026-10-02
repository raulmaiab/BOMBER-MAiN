#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include <stdbool.h>

#define LARGURA_GRADE_MAPA 17  //17 blocos de largura
#define ALTURA_GRADE_MAPA 11 //11 blocos de altura
#define TAMANHO_TILE 83       //Quantos pixels um tile tem

typedef enum {
    TILE_VAZIO,          
    TILE_INDESTRUTIVEL, 
    TILE_DESTRUTIVEL    
} TipoTile;

void InicializarMapa(const char* nomeTema);

void DesenharMapa(void);
void DescarregarMapa(void);

TipoTile ObterTipoTile(int x, int y);

void DefinirTipoTile(int x, int y, TipoTile novoTipo);

Vector2 ObterPosicaoInicialJogador(int indiceJogador);

Vector2 ObterPosGradeDePixels(Vector2 posicaoPixel);

#endif
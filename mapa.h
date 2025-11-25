#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include <stdbool.h>

// --- Constantes do Mapa ---
#define LARGURA_GRADE_MAPA 17  // 15 blocos de largura (17 incluindo bordas)
#define ALTURA_GRADE_MAPA 11 // 9 blocos de altura (11 incluindo bordas)
#define TAMANHO_TILE 83       // Tamanho do tile em pixels
// ---------------------------

typedef enum {
    TILE_VAZIO,          
    TILE_INDESTRUTIVEL, 
    TILE_DESTRUTIVEL    
} TipoTile;

/**
 * @brief Carrega texturas da pasta especificada e gera o layout.
 * @param nomeTema Nome da pasta onde estão os pngs (ex: "Default")
 */
void InicializarMapa(const char* nomeTema);

void DesenharMapa(void);
void DescarregarMapa(void);

/**
 * @brief Retorna o tipo de tile na posição de grade (x, y).
 * @param x Coordenada X da grade.
 * @param y Coordenada Y da grade.
 * @return Retorna TILE_INDESTRUTIVEL se estiver fora dos limites.
 */
TipoTile ObterTipoTile(int x, int y);

/**
 * @brief Define o tipo de tile na posição de grade (x, y).
 */
void DefinirTipoTile(int x, int y, TipoTile novoTipo);

/**
 * @brief Retorna a posição inicial em pixels para um jogador.
 */
Vector2 ObterPosicaoInicialJogador(int indiceJogador);

/**
 * @brief Converte coordenadas de pixel para coordenadas de grade.
 */
Vector2 ObterPosGradeDePixels(Vector2 posicaoPixel);

#endif // MAPA_H
#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include <stdbool.h>

// --- Constantes do Mapa ---
// --- ALTERADO PARA 15x11 (Tela 1440x900) ---
#define MAP_GRID_WIDTH 17  // 15 blocos de largura
#define MAP_GRID_HEIGHT 11 // 11 blocos de altura
#define TILE_SIZE 83      // 900 / 11 = 81.81 (usamos 81)
// --- FIM DA ALTERAÇÃO ---
// --- Tipos de Tiles ---
typedef enum {
    TILE_EMPTY,          // 0: Chão (por onde se anda)
    TILE_INDESTRUCTIBLE, // 1: Parede (fixa)
    TILE_DESTRUCTIBLE    // 2: Bloco (quebrável)
} TileType;

/**
 * @brief Carrega texturas (simuladas) e gera o layout do mapa.
 */
void InicializarMapa(void);

/**
 * @brief Desenha todo o grid do mapa na tela.
 */
void DesenharMapa(void);

/**
 * @brief Descarrega as texturas do mapa.
 */
void DescarregarMapa(void);

/**
 * @brief Retorna o tipo de tile em uma coordenada (x, y) do grid.
 */
TileType GetTileTipo(int x, int y);

/**
 * @brief Altera o tipo de tile (ex: quando uma bomba quebra um bloco).
 */
void SetTileTipo(int x, int y, TileType novoTipo);

/**
 * @brief Retorna a posição inicial (em pixels) para um jogador (0 a 3).
 */
Vector2 GetPlayerStartPosition(int playerIndex);

/**
 * @brief Converte uma posição do mundo (pixels) para coordenadas do grid.
 */
Vector2 GetGridPosFromPixels(Vector2 pixelPos);

#endif // MAPA_H
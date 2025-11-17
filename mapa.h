#ifndef MAPA_H
#define MAPA_H

#include "raylib.h"
#include <stdbool.h>

// --- Constantes do Mapa ---
#define MAP_GRID_WIDTH 17  // 15 blocos de largura
#define MAP_GRID_HEIGHT 11 // 11 blocos de altura
#define TILE_SIZE 83       // Tamanho do tile
// ---------------------------

typedef enum {
    TILE_EMPTY,          
    TILE_INDESTRUCTIBLE, 
    TILE_DESTRUCTIBLE    
} TileType;

/**
 * @brief Carrega texturas da pasta especificada e gera o layout.
 * @param pastaTema Nome da pasta onde estão os pngs (ex: "Default")
 */
void InicializarMapa(const char* pastaTema);

void DesenharMapa(void);
void DescarregarMapa(void);
TileType GetTileTipo(int x, int y);
void SetTileTipo(int x, int y, TileType novoTipo);
Vector2 GetPlayerStartPosition(int playerIndex);
Vector2 GetGridPosFromPixels(Vector2 pixelPos);

#endif // MAPA_H
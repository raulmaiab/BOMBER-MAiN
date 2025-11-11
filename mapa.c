#include "mapa.h"
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para srand()

// O grid 2D que armazena o estado de todo o mapa
static TileType mapa[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];

// Texturas simuladas (para não depender de imagens)
static Texture2D texChao;
static Texture2D texIndestrutivel;
static Texture2D texDestrutivel;

void InicializarMapa(void)
{
    srand(time(NULL)); 

    // 1. Gera texturas simuladas
    Image imgChao = GenImageColor(TILE_SIZE, TILE_SIZE, DARKGREEN);
    Image imgInd = GenImageColor(TILE_SIZE, TILE_SIZE, GRAY);
    Image imgDest = GenImageColor(TILE_SIZE, TILE_SIZE, LIME);
    ImageDrawRectangle(&imgDest, 5, 5, TILE_SIZE - 10, TILE_SIZE - 10, DARKBROWN);
    
    texChao = LoadTextureFromImage(imgChao);
    texIndestrutivel = LoadTextureFromImage(imgInd);
    texDestrutivel = LoadTextureFromImage(imgDest);
    
    UnloadImage(imgChao);
    UnloadImage(imgInd);
    UnloadImage(imgDest);

    // 2. Gera o layout do mapa
    for (int y = 0; y < MAP_GRID_HEIGHT; y++) {
        for (int x = 0; x < MAP_GRID_WIDTH; x++) {
            
            // Bordas Indestrutíveis
            if (y == 0 || y == MAP_GRID_HEIGHT - 1 || x == 0 || x == MAP_GRID_WIDTH - 1) {
                mapa[y][x] = TILE_INDESTRUCTIBLE;
            }
            // Padrão de xadrez de blocos indestrutíveis
            else if (y % 2 == 0 && x % 2 == 0) {
                mapa[y][x] = TILE_INDESTRUCTIBLE;
            }
            // O resto é chão
            else {
                mapa[y][x] = TILE_EMPTY;
            }
        }
    }

    // 3. Adiciona blocos destrutíveis aleatoriamente
    for (int y = 1; y < MAP_GRID_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_GRID_WIDTH - 1; x++) {
            if (mapa[y][x] == TILE_EMPTY) {
                if ((rand() % 100) < 75) { // 75% de chance
                    mapa[y][x] = TILE_DESTRUCTIBLE;
                }
            }
        }
    }

    // 4. Limpa as posições iniciais dos jogadores (extremidades)
    mapa[1][1] = TILE_EMPTY; // Canto Superior Esquerdo
    mapa[1][2] = TILE_EMPTY;
    mapa[2][1] = TILE_EMPTY;

    mapa[1][MAP_GRID_WIDTH - 2] = TILE_EMPTY; // Canto Superior Direito
    mapa[1][MAP_GRID_WIDTH - 3] = TILE_EMPTY;
    mapa[2][MAP_GRID_WIDTH - 2] = TILE_EMPTY;

    mapa[MAP_GRID_HEIGHT - 2][1] = TILE_EMPTY; // Canto Inferior Esquerdo
    mapa[MAP_GRID_HEIGHT - 2][2] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 3][1] = TILE_EMPTY;

    mapa[MAP_GRID_HEIGHT - 2][MAP_GRID_WIDTH - 2] = TILE_EMPTY; // Canto Inferior Direito
    mapa[MAP_GRID_HEIGHT - 2][MAP_GRID_WIDTH - 3] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 3][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
}

void DesenharMapa(void)
{
    for (int y = 0; y < MAP_GRID_HEIGHT; y++) {
        for (int x = 0; x < MAP_GRID_WIDTH; x++) {
            int posX = x * TILE_SIZE;
            int posY = y * TILE_SIZE;

            if (mapa[y][x] != TILE_INDESTRUCTIBLE) {
                DrawTexture(texChao, posX, posY, WHITE);
            }

            switch (mapa[y][x]) {
                case TILE_INDESTRUCTIBLE:
                    DrawTexture(texIndestrutivel, posX, posY, WHITE);
                    break;
                case TILE_DESTRUCTIBLE:
                    DrawTexture(texDestrutivel, posX, posY, WHITE);
                    break;
                case TILE_EMPTY:
                    break;
            }
        }
    }
}

void DescarregarMapa(void)
{
    UnloadTexture(texChao);
    UnloadTexture(texIndestrutivel);
    UnloadTexture(texDestrutivel);
}

TileType GetTileTipo(int x, int y)
{
    if (x < 0 || x >= MAP_GRID_WIDTH || y < 0 || y >= MAP_GRID_HEIGHT) {
        return TILE_INDESTRUCTIBLE; // Trata "fora do mapa" como uma parede
    }
    return mapa[y][x];
}

void SetTileTipo(int x, int y, TileType novoTipo)
{
    if (x >= 0 && x < MAP_GRID_WIDTH && y >= 0 && y < MAP_GRID_HEIGHT) {
        mapa[y][x] = novoTipo;
    }
}

Vector2 GetPlayerStartPosition(int playerIndex)
{
    // Retorna a posição em PIXELS
    switch (playerIndex) {
        case 0: return (Vector2){1 * TILE_SIZE, 1 * TILE_SIZE}; // Sup. Esq.
        case 1: return (Vector2){(MAP_GRID_WIDTH - 2) * TILE_SIZE, 1 * TILE_SIZE}; // Sup. Dir.
        case 2: return (Vector2){1 * TILE_SIZE, (MAP_GRID_HEIGHT - 2) * TILE_SIZE}; // Inf. Esq.
        case 3: return (Vector2){(MAP_GRID_WIDTH - 2) * TILE_SIZE, (MAP_GRID_HEIGHT - 2) * TILE_SIZE}; // Inf. Dir.
        default: return (Vector2){1 * TILE_SIZE, 1 * TILE_SIZE};
    }
}

Vector2 GetGridPosFromPixels(Vector2 pixelPos)
{
    // CORRIGIDO: Esta é uma conversão mais simples e útil
    // Apenas divide os pixels pelo tamanho do tile.
    int gridX = (int)(pixelPos.x / TILE_SIZE);
    int gridY = (int)(pixelPos.y / TILE_SIZE);
    
    return (Vector2){ (float)gridX, (float)gridY };
}
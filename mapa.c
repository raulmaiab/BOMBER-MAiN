#include "mapa.h"
#include <stdlib.h> 
#include <time.h>   
#include <stdio.h>  // Necessário para sprintf

static Texture2D texChao;
static Texture2D texIndestrutivel;
static Texture2D texDestrutivel;
static TileType mapa[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];

// Recebe o nome da pasta (ex: "Default" ou "Cave")
void InicializarMapa(const char* pastaTema)
{
    srand(time(NULL)); 

    char pathBuffer[256]; 

    // Constrói os caminhos baseados na pasta recebida
    sprintf(pathBuffer, "%s/ground.png", pastaTema);
    texChao = LoadTexture(pathBuffer);

    sprintf(pathBuffer, "%s/wall.png", pastaTema);
    texIndestrutivel = LoadTexture(pathBuffer);

    sprintf(pathBuffer, "%s/wallb.png", pastaTema);
    texDestrutivel = LoadTexture(pathBuffer); 

    // Logs de erro
    if (texChao.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar %s/ground.png", pastaTema);
    if (texIndestrutivel.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar %s/wall.png", pastaTema);
    if (texDestrutivel.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar %s/wallb.png", pastaTema);

    // Geração do layout (Sem alterações)
    for (int y = 0; y < MAP_GRID_HEIGHT; y++) {
        for (int x = 0; x < MAP_GRID_WIDTH; x++) {
            if (y == 0 || y == MAP_GRID_HEIGHT - 1 || x == 0 || x == MAP_GRID_WIDTH - 1) {
                mapa[y][x] = TILE_INDESTRUCTIBLE;
            } else if (y % 2 == 0 && x % 2 == 0) {
                mapa[y][x] = TILE_INDESTRUCTIBLE;
            } else {
                mapa[y][x] = TILE_EMPTY;
            }
        }
    }
    for (int y = 1; y < MAP_GRID_HEIGHT - 1; y++) {
        for (int x = 1; x < MAP_GRID_WIDTH - 1; x++) {
            if (mapa[y][x] == TILE_EMPTY) {
                if ((rand() % 100) < 75) {
                    mapa[y][x] = TILE_DESTRUCTIBLE;
                }
            }
        }
    }

    // Limpa posições iniciais
    mapa[1][1] = TILE_EMPTY; mapa[1][2] = TILE_EMPTY; mapa[2][1] = TILE_EMPTY;
    mapa[1][MAP_GRID_WIDTH - 2] = TILE_EMPTY; mapa[1][MAP_GRID_WIDTH - 3] = TILE_EMPTY; mapa[2][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 2][1] = TILE_EMPTY; mapa[MAP_GRID_HEIGHT - 2][2] = TILE_EMPTY; mapa[MAP_GRID_HEIGHT - 3][1] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 2][MAP_GRID_WIDTH - 2] = TILE_EMPTY; mapa[MAP_GRID_HEIGHT - 2][MAP_GRID_WIDTH - 3] = TILE_EMPTY; mapa[MAP_GRID_HEIGHT - 3][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
}

void DesenharMapa(void)
{
    Vector2 origin = { 0, 0 };
    Rectangle srcChao = { 0, 0, (float)texChao.width, (float)texChao.height };
    Rectangle srcInd = { 0, 0, (float)texIndestrutivel.width, (float)texIndestrutivel.height };
    Rectangle srcDest = { 0, 0, (float)texDestrutivel.width, (float)texDestrutivel.height };

    for (int y = 0; y < MAP_GRID_HEIGHT; y++) {
        for (int x = 0; x < MAP_GRID_WIDTH; x++) {
            Rectangle destRec = { (float)x * TILE_SIZE, (float)y * TILE_SIZE, (float)TILE_SIZE, (float)TILE_SIZE };
            DrawTexturePro(texChao, srcChao, destRec, origin, 0.0f, WHITE);

            switch (mapa[y][x]) {
                case TILE_INDESTRUCTIBLE:
                    DrawTexturePro(texIndestrutivel, srcInd, destRec, origin, 0.0f, WHITE);
                    break;
                case TILE_DESTRUCTIBLE:
                    if (texDestrutivel.id > 0) DrawTexturePro(texDestrutivel, srcDest, destRec, origin, 0.0f, WHITE);
                    else DrawRectangleRec(destRec, PURPLE);
                    break;
                case TILE_EMPTY: break;
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

TileType GetTileTipo(int x, int y) {
    if (x < 0 || x >= MAP_GRID_WIDTH || y < 0 || y >= MAP_GRID_HEIGHT) return TILE_INDESTRUCTIBLE;
    return mapa[y][x];
}

void SetTileTipo(int x, int y, TileType novoTipo) {
    if (x >= 0 && x < MAP_GRID_WIDTH && y >= 0 && y < MAP_GRID_HEIGHT) mapa[y][x] = novoTipo;
}

Vector2 GetPlayerStartPosition(int playerIndex) {
    switch (playerIndex) {
        case 0: return (Vector2){1 * TILE_SIZE, 1 * TILE_SIZE}; 
        case 1: return (Vector2){(MAP_GRID_WIDTH - 2) * TILE_SIZE, 1 * TILE_SIZE}; 
        case 2: return (Vector2){1 * TILE_SIZE, (MAP_GRID_HEIGHT - 2) * TILE_SIZE}; 
        case 3: return (Vector2){(MAP_GRID_WIDTH - 2) * TILE_SIZE, (MAP_GRID_HEIGHT - 2) * TILE_SIZE}; 
        default: return (Vector2){1 * TILE_SIZE, 1 * TILE_SIZE};
    }
}

Vector2 GetGridPosFromPixels(Vector2 pixelPos) {
    int gridX = (int)(pixelPos.x / TILE_SIZE);
    int gridY = (int)(pixelPos.y / TILE_SIZE);
    return (Vector2){ (float)gridX, (float)gridY };
}
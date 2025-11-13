#include "mapa.h"
#include <stdlib.h> 
#include <time.h>   

static Texture2D texChao;
static Texture2D texIndestrutivel;
static Texture2D texDestrutivel;

static TileType mapa[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];


void InicializarMapa(void)
{
    srand(time(NULL)); 

    texChao = LoadTexture("ground.png");
    texIndestrutivel = LoadTexture("wall.png");
    
    // --- O NOME DO SEU FICHEIRO TEM QUE SER EXATAMENTE ESTE ---
    texDestrutivel = LoadTexture("wallb.png"); 

    if (texChao.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar ground.png");
    if (texIndestrutivel.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar wall.png");
    
    // Se esta mensagem aparecer no terminal, o nome está errado:
    if (texDestrutivel.id == 0) TraceLog(LOG_WARNING, "FALHA AO CARREGAR: wallb.png");
    
    // Geração do Mapa (Sem alteração)
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

    // Limpa Spawn Points (Sem alteração)
    mapa[1][1] = TILE_EMPTY;
    mapa[1][2] = TILE_EMPTY;
    mapa[2][1] = TILE_EMPTY;
    mapa[1][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
    mapa[1][MAP_GRID_WIDTH - 3] = TILE_EMPTY;
    mapa[2][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 2][1] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 2][2] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 3][1] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 2][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 2][MAP_GRID_WIDTH - 3] = TILE_EMPTY;
    mapa[MAP_GRID_HEIGHT - 3][MAP_GRID_WIDTH - 2] = TILE_EMPTY;
}

// --- ATUALIZADO COM DEBUG ---
void DesenharMapa(void)
{
    Vector2 origin = { 0, 0 };

    Rectangle srcChao = { 0, 0, (float)texChao.width, (float)texChao.height };
    Rectangle srcInd = { 0, 0, (float)texIndestrutivel.width, (float)texIndestrutivel.height };
    Rectangle srcDest = { 0, 0, (float)texDestrutivel.width, (float)texDestrutivel.height };

    for (int y = 0; y < MAP_GRID_HEIGHT; y++) {
        for (int x = 0; x < MAP_GRID_WIDTH; x++) {
            
            Rectangle destRec = { 
                (float)x * TILE_SIZE, 
                (float)y * TILE_SIZE, 
                (float)TILE_SIZE, 
                (float)TILE_SIZE 
            };

            DrawTexturePro(texChao, srcChao, destRec, origin, 0.0f, WHITE);

            switch (mapa[y][x]) {
                case TILE_INDESTRUCTIBLE:
                    DrawTexturePro(texIndestrutivel, srcInd, destRec, origin, 0.0f, WHITE);
                    break;
                case TILE_DESTRUCTIBLE:
                    
                    // --- CORREÇÃO DE DEBUG ---
                    if (texDestrutivel.id > 0) {
                        // Se a textura carregou, desenha o PNG
                        DrawTexturePro(texDestrutivel, srcDest, destRec, origin, 0.0f, WHITE);
                    } else {
                        // Se falhou, desenha um bloco ROXO para vermos o bug
                        DrawRectangleRec(destRec, PURPLE);
                    }
                    // --- FIM DA CORREÇÃO ---
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

// --- Funções Get/Set (Sem alteração) ---
TileType GetTileTipo(int x, int y) {
    if (x < 0 || x >= MAP_GRID_WIDTH || y < 0 || y >= MAP_GRID_HEIGHT) {
        return TILE_INDESTRUCTIBLE;
    }
    return mapa[y][x];
}
void SetTileTipo(int x, int y, TileType novoTipo) {
    if (x >= 0 && x < MAP_GRID_WIDTH && y >= 0 && y < MAP_GRID_HEIGHT) {
        mapa[y][x] = novoTipo;
    }
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
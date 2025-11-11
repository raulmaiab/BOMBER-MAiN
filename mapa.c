#include "mapa.h"
#include <stdlib.h> // Para rand() e srand()
#include <time.h>   // Para srand()

// --- ALTERADO: Texturas Mistas (Simulada e Carregadas) ---
static Texture2D texChao;          // Textura simulada (verde)
static Texture2D texIndestrutivel; // Carregada de wall.png
static Texture2D texDestrutivel;   // Carregada de wallb.png
// --- FIM DA ALTERAÇÃO ---

// O grid do mapa (sem alteração)
static TileType mapa[MAP_GRID_HEIGHT][MAP_GRID_WIDTH];


void InicializarMapa(void)
{
    srand(time(NULL)); 

    // --- ALTERADO: Carregar texturas ---
    
    // 1. Gera a textura do CHÃO (simulada)
    Image imgChao = GenImageColor(TILE_SIZE, TILE_SIZE, DARKGREEN);
    texChao = LoadTextureFromImage(imgChao);
    UnloadImage(imgChao); // Já podemos descarregar a imagem da CPU

    // 2. Carrega as texturas das PAREDES (dos PNGs)
    texIndestrutivel = LoadTexture("wall.png");
    texDestrutivel = LoadTexture("wallb.png"); // Usando o nome que você especificou

    // 3. Checagem de falha
    if (texChao.id == 0) TraceLog(LOG_WARNING, "Falha ao gerar textura do chão");
    if (texIndestrutivel.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar wall.png");
    if (texDestrutivel.id == 0) TraceLog(LOG_WARNING, "Falha ao carregar wallb.png");
    
    // --- FIM DA ALTERAÇÃO ---


    // 3. Gera o layout do mapa (Sem alteração)
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

    // 4. Limpa as posições iniciais (Sem alteração)
    // (O código dos 4 cantos permanece o mesmo)
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

void DesenharMapa(void)
{
    // --- ALTERADO: Lógica de desenho mista ---
    
    Vector2 origin = { 0, 0 };

    // Retângulos Fonte (Source)
    // O chão simulado tem o tamanho exato de TILE_SIZE
    Rectangle srcChao = { 0, 0, (float)texChao.width, (float)texChao.height };
    // As paredes (wall.png/wallb.png) têm suas próprias dimensões (ex: 16x16)
    Rectangle srcInd = { 0, 0, (float)texIndestrutivel.width, (float)texIndestrutivel.height };
    Rectangle srcDest = { 0, 0, (float)texDestrutivel.width, (float)texDestrutivel.height };


    for (int y = 0; y < MAP_GRID_HEIGHT; y++) {
        for (int x = 0; x < MAP_GRID_WIDTH; x++) {
            
            // Retângulo Destino (onde vai na tela)
            // Todos serão desenhados no tamanho 90x90
            Rectangle destRec = { 
                (float)x * TILE_SIZE, 
                (float)y * TILE_SIZE, 
                (float)TILE_SIZE, 
                (float)TILE_SIZE 
            };

            // Desenha o CHÃO (verde simulado) por baixo de tudo
            DrawTexturePro(texChao, srcChao, destRec, origin, 0.0f, WHITE);

            // Desenha a parede (PNG) por CIMA do chão
            switch (mapa[y][x]) {
                case TILE_INDESTRUCTIBLE:
                    DrawTexturePro(texIndestrutivel, srcInd, destRec, origin, 0.0f, WHITE);
                    break;
                case TILE_DESTRUCTIBLE:
                    DrawTexturePro(texDestrutivel, srcDest, destRec, origin, 0.0f, WHITE);
                    break;
                case TILE_EMPTY:
                    // O chão já foi desenhado
                    break;
            }
        }
    }
    // --- FIM DA ALTERAÇÃO ---
}

void DescarregarMapa(void)
{
    // --- ALTERADO: Descarregar as 3 texturas ---
    UnloadTexture(texChao);
    UnloadTexture(texIndestrutivel);
    UnloadTexture(texDestrutivel);
}

// --- Funções Get/Set (Sem alteração) ---

TileType GetTileTipo(int x, int y)
{
    if (x < 0 || x >= MAP_GRID_WIDTH || y < 0 || y >= MAP_GRID_HEIGHT) {
        return TILE_INDESTRUCTIBLE;
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
    switch (playerIndex) {
        case 0: return (Vector2){1 * TILE_SIZE, 1 * TILE_SIZE}; 
        case 1: return (Vector2){(MAP_GRID_WIDTH - 2) * TILE_SIZE, 1 * TILE_SIZE}; 
        case 2: return (Vector2){1 * TILE_SIZE, (MAP_GRID_HEIGHT - 2) * TILE_SIZE}; 
        case 3: return (Vector2){(MAP_GRID_WIDTH - 2) * TILE_SIZE, (MAP_GRID_HEIGHT - 2) * TILE_SIZE};
        default: return (Vector2){1 * TILE_SIZE, 1 * TILE_SIZE};
    }
}

Vector2 GetGridPosFromPixels(Vector2 pixelPos)
{
    int gridX = (int)(pixelPos.x / TILE_SIZE);
    int gridY = (int)(pixelPos.y / TILE_SIZE);
    return (Vector2){ (float)gridX, (float)gridY };
}
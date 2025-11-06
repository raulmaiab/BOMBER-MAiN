#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

#include "raylib.h" // Essencial, pois nossas structs usam Vector2

// --- Constantes do Jogo (#defines) ---
#define MAP_LARGURA 15
#define MAP_ALTURA 11
#define TAMANHO_TILE 40 // 40 pixels por quadrado

// Janela (derivada dos #defines)
const int screenWidth = MAP_LARGURA * TAMANHO_TILE;
const int screenHeight = MAP_ALTURA * TAMANHO_TILE;

// --- Enums do Mapa (Matriz) ---
typedef enum {
    TILE_VAZIO = 0,
    TILE_PAREDE_FIXA,
    TILE_PAREDE_MACIA
} TipoTile;

// --- Structs ---
typedef struct {
    Vector2 pos; // Posição em PIXELS
    float velocidade;
    int poder_explosao;
    int max_bombas;
    int bombas_plantadas;
} Jogador;

typedef struct {
    Vector2 pos_grid; // Posição na GRADE
    float timer;
    int poder;
} Bomba;

typedef struct {
    Vector2 pos_grid; // Posição na GRADE
    float timer;      // Duração da explosão
} Explosao;


// --- Listas Encadeadas (Structs de Nó) ---
typedef struct NodeBomba {
    Bomba bomba;
    struct NodeBomba* proximo;
} NodeBomba;

typedef struct NodeExplosao {
    Explosao explosao;
    struct NodeExplosao* proximo;
} NodeExplosao;

// --- Protótipos de Funções ---
// "Promessas" das funções que estarão em bomberman.c
void InitGame(void);
void UpdateGame(void);
void DrawGame(void);
void UnloadGame(void);
void PlantarBomba(Vector2 pos_grid);
void CriarExplosoes(Vector2 pos_grid, int poder);
void DesenharMapa(void);

#endif // BOMBERMAN_H_
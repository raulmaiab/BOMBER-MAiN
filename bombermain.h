#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_

#include "raylib.h" 

// --- Constantes Virtuais e de Mapa ---
// (Removemos JANELA_LARGURA e JANELA_ALTURA)

#define MAP_LARGURA 15
#define MAP_ALTURA 13
#define TAMANHO_TILE 40

// Esta é a resolução INTERNA do seu jogo
#define VIRTUAL_LARGURA (MAP_LARGURA * TAMANHO_TILE) // 600
#define VIRTUAL_ALTURA (MAP_ALTURA * TAMANHO_TILE) // 520

#define SPRITE_LARGURA 16 
#define SPRITE_ALTURA 24

// --- Structs e Enums ---

// Telas e Cores
typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_FIM
} Tela;

typedef enum {
    BOMBERMAN_BRANCO = 0,
    BOMBERMAN_PRETO,
    BOMBERMAN_AZUL,
    BOMBERMAN_VERMELHO,
    NUM_BOMBERMAN_CORES
} BombermanCor;

// --- Entidades do Jogo (Dados) ---

typedef struct {
    Vector2 posicao;
    Texture2D boneco_sprite;
    BombermanCor cor;
    int vida;
    int poder_bomba;
    int max_bombas;
    int bombas_plantadas;
    bool bot;
    // Animação
    int frame_atual;
    float frame_timer;
    Vector2 direcao_olhando;
} Jogador;

typedef struct {
    Vector2 posicao_bomba; // Posição no GRID
    int poder;
    float temporizador;
} Bomba;

typedef struct {
    Vector2 pos_grid;
    float timer; // Duração
} Explosao;

typedef struct {
    Vector2 pos_grid;
    int tipo; // 0 = +bomba, 1 = +poder
} Item;

// --- Nós das Listas Encadeadas (Para malloc/free) ---

typedef struct NodeBomba {
    Bomba bomba;
    struct NodeBomba* proximo;
} NodeBomba;

typedef struct NodeExplosao {
    Explosao explosao;
    struct NodeExplosao* proximo;
} NodeExplosao;

typedef struct NodeItem {
    Item item;
    struct NodeItem* proximo;
} NodeItem;


// --- Protótipos de Funções (O "Contrato") ---

// Funções de Gerenciamento
void IniciarJogo(void);
void AtualizarJogo(void);
void DesenharJogo(void);
void FinalizarJogo(void);

// Funções das Telas
void AtualizarTelaMenu(void);
void DesenharTelaMenu(void);
void AtualizarTelaJogo(void);
void DesenharTelaJogo(void);
void AtualizarTelaFim(void);
void DesenharTelaFim(void);

// Funções de Lógica da Partida
void ReiniciarMapa(void);
void ProcessarTecla(void);
void AtualizarMovimento(void);
void PlantarBomba(int id_jogador);
void AtualizarBombas(float dt);
void CriarExplosao(Vector2 pos_grid, int poder);
void GerarItem(Vector2 pos_grid);

#endif // BOMBERMAN_H_
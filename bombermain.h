#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_
#define MAP_LARGURA 15
#define MAP_ALTURA 13
#define TAMANHO_TILE 40
#include "raylib.h" 


//Definindo jogadores, bombas e telas

#define SPRITE_LARGURA 16 
#define SPRITE_ALTURA 24

typedef enum {
    BOMBERMAN_BRANCO = 0, // Primeiro jogador na primeira linha
    BOMBERMAN_PRETO,      // Segundo jogador na segunda linha
    BOMBERMAN_AZUL,       // Terceiro jogador na terceira linha
    BOMBERMAN_VERMELHO,   // Quarto jogador na quarta linha
    NUM_BOMBERMAN_CORES   // Ajuda a saber quantas cores temos
} BombermanCor;

typedef struct {
    Vector2 posicao;
    Texture2D boneco_sprite;
    BombermanCor cor;
    int vida ;
    int poder_bomba;
    int max_bombas;
    int bombas_plantadas;
    bool bot;

    int frame_atual;        
    float frame_timer;       
    Vector2 direcao_olhando;
    

}Jogador;

typedef struct {
    Vector2 posicao_bomba;
    Texture2D bomba;
    int poder;
    float temporizador;
}Bomba;

typedef struct {
    Vector2 pos_grid;
    float timer; // Duração
} Explosao;

typedef struct {
    Vector2 pos_grid;
    int tipo; 
} Item;

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

typedef enum {
    TELA_MENU,
    TELA_JOGO,
    TELA_FIM
} Tela;

//1. Funções de começo
void IniciarJogo(void);     //Carrega texturas, inicializa Raylib

void AtualizarJogo(void);   //Atualiza 1 frame

void DesenharJogo(void);    //Desenha 1 frame

void FinalizarJogo(void);   //Libera memória (free)

//2. Funções das telas

//Menu:
void AtualizarTelaMenu(void);
void DesenharTelaMenu(void);

//Partida:
void AtualizarTelaJogo(void);
void DesenharTelaJogo(void);

//Tela_Final:
void AtualizarTelaFim(void);
void DesenharTelaFim(void);


//Funções do bombermain.c
void ReiniciarMapa(void);                         // Limpa o mapa e coloca os blocos 
void ProcessarTecla(void);                        // Recebe as teclas (W,A,S,D, Espaço)
void AtualizarMovimento(void);                    // Move jogadores e verifica colisões
void PlantarBomba(int id_jogador);                // Cria uma nova bomba (com malloc)
void AtualizarBombas(float dt);                   // Atualiza timers e explode
void CriarExplosao(Vector2 pos_grid, int poder);  // Cria a explosão
void GerarItem(Vector2 pos_grid);                 // Gera itens ao quebrar paredes

#endif // BOMBERMAN_H_
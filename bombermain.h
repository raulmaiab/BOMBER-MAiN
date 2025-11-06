#ifndef BOMBERMAN_H_
#define BOMBERMAN_H_
#define MAP_LARGURA 15
#define MAP_ALTURA 13
#define TAMANHO_TILE 40
#include "raylib.h" 


//Definindo jogadores, bombas e 

typedef struct {
    Vector2 posicao;
    Texture2D boneco;
    int vida ;
    int poder_bomba;
    int max_bombas;
    int bombas_plantadas;
    bool bot;

}Jogador;

typedef struct {
    Vector2 posicao_bomba;
    Texture2D bomba;
    int poder;
    float temporizador;
}Bomba;

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




// 1.Iniciar jogo
// Rodar...

//Escolhas (Menu)
// Quantos jogadores irão participar (1: 1 player x 3 bots) (2: 2 players x 2 bots) (3: 3 players x 1 bot) (4: 4 players)
// Modo de jogo (Equipe ou Normal)

// Escolha de boneco

//2.Execução da partida (mapa 13 x 15)

//Conseguir receber as  teclas para movimento e para colocar as bombas

//Ter paredes destrutíveis

//Gerar itens que, ao serem colotados, aumentam o tamanho da explosão

//Tela de fim de jogo ao sobrar um jogador, menu (jogar novamente ou voltar ao menu)

//Reiniciar o mapa ao jogar novamente
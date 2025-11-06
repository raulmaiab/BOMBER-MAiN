// 1. Inclui o cabeçalho do nosso próprio projeto
#include "bombermain.h"

// 2. Inclui outras bibliotecas necessárias para a implementação
#include <stdlib.h> // Para malloc, free, rand
#include <stdio.h>  // Para printf (debug)
#include <math.h>   // Para roundf (arredondar pos do jogador)

// --- Matriz Global ---
int g_mapa[MAP_ALTURA][MAP_LARGURA];

// --- Ponteiros (Listas Globais) ---
Jogador g_jogador;
NodeBomba* g_listaBombas = NULL;
NodeExplosao* g_listaExplosoes = NULL;

// --- Função Principal ---
int main(void) {
    InitGame();

    while (!WindowShouldClose()) {
        UpdateGame();
        DrawGame();
    }

    UnloadGame();
    return 0;
}

// --- Implementações das Funções ---

void InitGame(void) {
    InitWindow(screenWidth, screenHeight, "Bomberman Raylib");
    SetTargetFPS(60);

    // Inicializa o Jogador
    g_jogador.pos = (Vector2){ TAMANHO_TILE * 1.5f, TAMANHO_TILE * 1.5f };
    g_jogador.velocidade = 150.0f;
    g_jogador.max_bombas = 1;
    g_jogador.bombas_plantadas = 0;
    g_jogador.poder_explosao = 2;

    // Inicializa o Mapa (Matriz)
    for (int y = 0; y < MAP_ALTURA; y++) {
        for (int x = 0; x < MAP_LARGURA; x++) {
            if (y == 0 || x == 0 || y == MAP_ALTURA - 1 || x == MAP_LARGURA - 1) {
                g_mapa[y][x] = TILE_PAREDE_FIXA; // Bordas
            }
            else if (y % 2 == 0 && x % 2 == 0) {
                g_mapa[y][x] = TILE_PAREDE_FIXA; // Pilares
            }
            else if (rand() % 10 < 7) {
                g_mapa[y][x] = TILE_PAREDE_MACIA; // 70% chance de parede macia
            } else {
                g_mapa[y][x] = TILE_VAZIO;
            }
        }
    }
    // Limpa a área inicial do jogador
    g_mapa[1][1] = TILE_VAZIO;
    g_mapa[1][2] = TILE_VAZIO;
    g_mapa[2][1] = TILE_VAZIO;
}

void UpdateGame(void) {
    float dt = GetFrameTime();
    Vector2 pos_antiga = g_jogador.pos;

    // --- Input do Jogador ---
    if (IsKeyDown(KEY_W)) g_jogador.pos.y -= g_jogador.velocidade * dt;
    if (IsKeyDown(KEY_S)) g_jogador.pos.y += g_jogador.velocidade * dt;
    if (IsKeyDown(KEY_A)) g_jogador.pos.x -= g_jogador.velocidade * dt;
    if (IsKeyDown(KEY_D)) g_jogador.pos.x += g_jogador.velocidade * dt;

    // --- Colisão (Simples, baseada em grid) ---
    Vector2 pos_grid_jogador = {
        roundf(g_jogador.pos.x / TAMANHO_TILE),
        roundf(g_jogador.pos.y / TAMANHO_TILE)
    };
    if (g_mapa[(int)pos_grid_jogador.y][(int)pos_grid_jogador.x] != TILE_VAZIO) {
        g_jogador.pos = pos_antiga; // Desfaz o movimento se colidir
    }

    // Plantar Bomba (usando malloc)
    if (IsKeyPressed(KEY_SPACE) && g_jogador.bombas_plantadas < g_jogador.max_bombas) {
        Vector2 pos_grid_bomba = {
            roundf(g_jogador.pos.x / TAMANHO_TILE),
            roundf(g_jogador.pos.y / TAMANHO_TILE)
        };
        PlantarBomba(pos_grid_bomba);
    }

    // --- Atualizar Bombas (Iterando a Lista Encadeada) ---
    NodeBomba** ptrBomba = &g_listaBombas;
    while (*ptrBomba) {
        NodeBomba* noAtual = *ptrBomba;
        noAtual->bomba.timer -= dt;

        if (noAtual->bomba.timer <= 0) {
            CriarExplosoes(noAtual->bomba.pos_grid, noAtual->bomba.poder);
            *ptrBomba = noAtual->proximo;
            free(noAtual);
            g_jogador.bombas_plantadas--;
        } else {
            ptrBomba = &noAtual->proximo;
        }
    }
    
    // --- Atualizar Explosões (Iterando a Lista) ---
    NodeExplosao** ptrExplosao = &g_listaExplosoes;
    while (*ptrExplosao) {
        NodeExplosao* noAtual = *ptrExplosao;
        noAtual->explosao.timer -= dt;
        
        if (noAtual->explosao.timer <= 0) {
            *ptrExplosao = noAtual->proximo;
            free(noAtual);
        } else {
            ptrExplosao = &noAtual->proximo;
        }
    }
}

void DrawGame(void) {
    BeginDrawing();
    ClearBackground(DARKGREEN);

    DesenharMapa();

    // Desenha Bombas
    NodeBomba* bombaAtual = g_listaBombas;
    while (bombaAtual) {
        DrawRectangle(
            bombaAtual->bomba.pos_grid.x * TAMANHO_TILE,
            bombaAtual->bomba.pos_grid.y * TAMANHO_TILE,
            TAMANHO_TILE, TAMANHO_TILE, BLACK
        );
        bombaAtual = bombaAtual->proximo;
    }

    // Desenha Explosões
    NodeExplosao* explosaoAtual = g_listaExplosoes;
    while (explosaoAtual) {
        DrawRectangle(
            explosaoAtual->explosao.pos_grid.x * TAMANHO_TILE,
            explosaoAtual->explosao.pos_grid.y * TAMANHO_TILE,
            TAMANHO_TILE, TAMANHO_TILE, ORANGE
        );
        explosaoAtual = explosaoAtual->proximo;
    }

    // Desenha Jogador
    DrawRectangle(g_jogador.pos.x - TAMANHO_TILE/2, g_jogador.pos.y - TAMANHO_TILE/2, TAMANHO_TILE, TAMANHO_TILE, BLUE);

    EndDrawing();
}

void DesenharMapa(void) {
    for (int y = 0; y < MAP_ALTURA; y++) {
        for (int x = 0; x < MAP_LARGURA; x++) {
            if (g_mapa[y][x] == TILE_PAREDE_FIXA) {
                DrawRectangle(x * TAMANHO_TILE, y * TAMANHO_TILE, TAMANHO_TILE, TAMANHO_TILE, GRAY);
            } else if (g_mapa[y][x] == TILE_PAREDE_MACIA) {
                DrawRectangle(x * TAMANHO_TILE, y * TAMANHO_TILE, TAMANHO_TILE, TAMANHO_TILE, BROWN);
            }
        }
    }
}

void PlantarBomba(Vector2 pos_grid) {
    NodeBomba* novo_no = (NodeBomba*)malloc(sizeof(NodeBomba));
    if (novo_no == NULL) return;

    novo_no->bomba.pos_grid = pos_grid;
    novo_no->bomba.timer = 3.0f;
    novo_no->bomba.poder = g_jogador.poder_explosao;
    novo_no->proximo = g_listaBombas;
    g_listaBombas = novo_no;
    g_jogador.bombas_plantadas++;
}

void CriarExplosoes(Vector2 pos_grid, int poder) {
    // 1. Aloca (malloc)
    NodeExplosao* novo_no = (NodeExplosao*)malloc(sizeof(NodeExplosao));
    if (novo_no == NULL) return;

    // 2. Preenche dados
    novo_no->explosao.pos_grid = pos_grid;
    novo_no->explosao.timer = 0.5f; // Meio segundo de duração
    novo_no->proximo = g_listaExplosoes;
    g_listaExplosoes = novo_no;
    
    // Destrói paredes macias
    if (g_mapa[(int)pos_grid.y][(int)pos_grid.x] == TILE_PAREDE_MACIA) {
         g_mapa[(int)pos_grid.y][(int)pos_grid.x] = TILE_VAZIO;
    }
    
    // (A lógica de propagação da explosão ainda precisa ser implementada)
}

void UnloadGame(void) {
    // Libera toda a memória das listas encadeadas (free)
    NodeBomba* bombaAtual = g_listaBombas;
    while (bombaAtual) {
        NodeBomba* proximo = bombaAtual->proximo;
        free(bombaAtual);
        bombaAtual = proximo;
    }
    
    NodeExplosao* explosaoAtual = g_listaExplosoes;
    while (explosaoAtual) {
        NodeExplosao* proximo = explosaoAtual->proximo;
        free(explosaoAtual);
        explosaoAtual = proximo;
    }

    CloseWindow();
}
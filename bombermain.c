#include "bombermain.h"  // O seu cabeçalho
#include <stdlib.h>     // Para malloc, free, rand
#include <math.h>       // Para roundf (colisão)
#include <stdio.h>      // Para printf (debug)

// --- Variáveis Globais ---
Tela g_tela_atual;
int g_mapa[MAP_ALTURA][MAP_LARGURA];
Jogador g_jogadores[4];

// Listas Encadeadas (começam vazias)
NodeBomba* g_lista_bombas = NULL;
NodeExplosao* g_lista_explosoes = NULL;
NodeItem* g_lista_itens = NULL;

// Texturas Globais
Texture2D g_spritesheet_personagens;
Texture2D g_sprite_bomba;
Texture2D g_sprite_parede_fixa;
Texture2D g_sprite_parede_macia;
// ... (outras texturas que você precisar) ...

// Array "mágico" para os offsets Y do spritesheet
const int Y_OFFSET_COR[] = { 0, 80, 160, 240 };


// --- Função Principal ---
int main(void) {
    IniciarJogo();

    // O "Rodar..."
    while (!WindowShouldClose()) {
        AtualizarJogo();
        DesenharJogo();
    }

    FinalizarJogo();
    return 0;
}

// --- Implementação das Funções ---

// 1. Funções de Gerenciamento
void IniciarJogo(void) {
    InitWindow(MAP_LARGURA * TAMANHO_TILE, MAP_ALTURA * TAMANHO_TILE, "Bomberman");
    SetTargetFPS(60);

    // --- Carregar Sprites ---
    // (Lembre de renomear seu arquivo para 'personagens.png')
    g_spritesheet_personagens = LoadTexture("personagens.png"); 
    // g_sprite_bomba = LoadTexture("bomba.png");
    // g_sprite_parede_macia = LoadTexture("parede_macia.png");
    // ... carregar todas as suas texturas ...

    // Configurar jogadores
    for (int i = 0; i < 4; i++) {
        g_jogadores[i].boneco_sprite = g_spritesheet_personagens;
        g_jogadores[i].cor = (BombermanCor)i; // Branco, Preto, Azul, Vermelho
        g_jogadores[i].posicao = (Vector2){ TAMANHO_TILE * 1.5f + (i * TAMANHO_TILE * 3), TAMANHO_TILE * 1.5f };
        g_jogadores[i].frame_atual = 0;
        g_jogadores[i].direcao_olhando = (Vector2){0, 1}; // Olhando para baixo
        g_jogadores[i].vida = 1;
        g_jogadores[i].max_bombas = 1;
        g_jogadores[i].poder_bomba = 1;
        // ... (etc)
    }

    g_tela_atual = TELA_MENU; // Começa no menu
    // ReiniciarMapa();
}

void AtualizarJogo(void) {
    switch (g_tela_atual) {
        case TELA_MENU:
            AtualizarTelaMenu();
            break;
        case TELA_JOGO:
            AtualizarTelaJogo();
            break;
        case TELA_FIM:
            AtualizarTelaFim();
            break;
    }
}

void DesenharJogo(void) {
    BeginDrawing();
    ClearBackground(DARKGREEN); // Cor do chão

    switch (g_tela_atual) {
        case TELA_MENU:
            DesenharTelaMenu();
            break;
        case TELA_JOGO:
            DesenharTelaJogo();
            break;
        case TELA_FIM:
            DesenharTelaFim();
            break;
    }
    
    EndDrawing();
}

void FinalizarJogo(void) {
    // --- Descarregar Sprites ---
    UnloadTexture(g_spritesheet_personagens);
    // UnloadTexture(g_sprite_bomba);
    // ... (descarregar todas as texturas)

    // TODO: Limpar listas encadeadas (com free())
    
    CloseWindow();
}

// 2. Funções das Telas

// --- Menu ---
void AtualizarTelaMenu(void) {
    // Lógica para seleção de personagem, jogadores, etc.
    if (IsKeyPressed(KEY_ENTER)) {
        g_tela_atual = TELA_JOGO;
        // ReiniciarMapa(); // Prepara o mapa para a partida
    }
}

void DesenharTelaMenu(void) {
    DrawText("BOMBERMAN", 200, 150, 40, WHITE);
    DrawText("Pressione ENTER para jogar", 190, 250, 20, WHITE);
}

// --- Jogo (Partida) ---
void AtualizarTelaJogo(void) {
    ProcessarTecla();
    AtualizarMovimento();
    AtualizarBombas(GetFrameTime());
    // TODO: AtualizarExplosoes, Colisões, etc.
}

// Função auxiliar (protótipo não precisa estar no .h)
void DesenharJogadores(void);
void DesenharMapa(void);
void DesenharBombas(void);

void DesenharTelaJogo(void) {
    DesenharMapa();
    // DesenharItens();
    DesenharBombas();
    // DesenharExplosoes();
    DesenharJogadores(); // <- Usa a função de sprite
}

// --- Tela Final ---
void AtualizarTelaFim(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        g_tela_atual = TELA_MENU;
    }
}

void DesenharTelaFim(void) {
    DrawText("FIM DE JOGO", 200, 150, 40, WHITE);
    DrawText("Pressione ENTER para voltar ao menu", 150, 250, 20, WHITE);
}

// 3. Funções de Lógica da Partida

void ReiniciarMapa(void) {
    // TODO: Usar 'rand()' para popular g_mapa com TILE_PAREDE_MACIA
}

void ProcessarTecla(void) {
    // TODO: Mudar a 'posicao' do g_jogadores[0] (jogador 1)
    // if (IsKeyDown(KEY_W)) { ... }
    // if (IsKeyPressed(KEY_SPACE)) { PlantarBomba(0); }
}

void AtualizarMovimento(void) {
    // TODO: Lógica de animação e colisão com g_mapa
    // Pega o dt = GetFrameTime();
    // para cada jogador:
    //   jogador->frame_timer += dt;
    //   if (jogador->frame_timer >= 0.1f) {
    //      jogador->frame_timer = 0;
    //      jogador->frame_atual = (jogador->frame_atual + 1) % 3; // 3 frames de anim
    //   }
}

void PlantarBomba(int id_jogador) {
    // TODO:
    // 1. Verificar se g_jogadores[id_jogador].bombas_plantadas < ...max_bombas
    // 2. Alocar memória (NodeBomba* nova_bomba = malloc(...))
    // 3. Preencher dados (pos, poder, timer=3.0f)
    // 4. Adicionar na lista g_lista_bombas
}

void AtualizarBombas(float dt) {
    // TODO:
    // 1. Percorrer a lista g_lista_bombas
    // 2. Para cada bomba: bomba->temporizador -= dt;
    // 3. Se temporizador <= 0:
    //    CriarExplosao(...)
    //    Remover a bomba da lista (usando free())
}

void CriarExplosao(Vector2 pos_grid, int poder) {
    // TODO:
    // 1. Alocar memória (malloc) para NodeExplosao
    // 2. Adicionar na lista g_lista_explosoes (com timer curto, ex: 0.5f)
    // 3. (Lógica de propagação):
    //    Quebrar TILE_PAREDE_MACIA no g_mapa
    //    Chamar GerarItem(...)
}

void GerarItem(Vector2 pos_grid) {
    // TODO:
    // 1. Sorteia (rand()) se vai gerar item
    // 2. Se sim, aloca (malloc) para NodeItem
    // 3. Adiciona na lista g_lista_itens
}


// --- Implementação das funções de desenho ---

void DesenharMapa(void) {
    // TODO: Loop for duplo em g_mapa e desenhar retângulos
    // ou DrawTexture(g_sprite_parede_fixa, ...)
}

void DesenharBombas(void) {
    // TODO: Percorrer g_lista_bombas e desenhar
    // DrawTexture(g_sprite_bomba, ...)
}

void DesenharJogadores(void) {
    for (int i = 0; i < 4; i++) {
        // if (g_jogadores[i].vida <= 0) continue; 
        Jogador* jog = &g_jogadores[i];

        // 1. Recorte (Source)
        Rectangle sourceRec = { 0 };
        sourceRec.x = jog->frame_atual * SPRITE_LARGURA;
        sourceRec.y = Y_OFFSET_COR[jog->cor];
        sourceRec.width = SPRITE_LARGURA;
        sourceRec.height = SPRITE_ALTURA;

        // 2. Destino (Dest)
        // (Ajuste fino da posição para centralizar o sprite)
        Vector2 destPos = {
            jog->posicao.x - (TAMANHO_TILE / 2) + 4.0f, // +4 para centralizar o 16px no 40px
            jog->posicao.y - (TAMANHO_TILE / 2) - 4.0f  // -4 para "subir" o sprite
        };

        // 3. Desenha!
        DrawTextureRec(g_spritesheet_personagens, sourceRec, destPos, WHITE);
    }
}
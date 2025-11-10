#include "bombermain.h"  // O seu cabeçalho
#include <stdlib.h>     // Para malloc, free, rand
#include <math.h>       // Para roundf, fminf
#include <stdio.h>      // Para printf (debug)

// --- Variáveis Globais ---
Tela g_tela_atual;
int g_mapa[MAP_ALTURA][MAP_LARGURA];
Jogador g_jogadores[4];

// Listas Encadeadas
NodeBomba* g_lista_bombas = NULL;
NodeExplosao* g_lista_explosoes = NULL;
NodeItem* g_lista_itens = NULL;

// Texturas Globais
Texture2D g_textura_tela_inicio; 
Texture2D g_sprite_bomba;
Texture2D g_sprite_parede_fixa;
Texture2D g_sprite_parede_macia;

// --- Variáveis de Escala (O "Canvas Virtual") ---
RenderTexture2D g_canvas_virtual; // O canvas interno de 600x520
Rectangle g_canvas_origem;      // Onde cortar do canvas
Rectangle g_canvas_destino;     // Onde desenhar na tela (com barras pretas)
float g_escala;                 // O fator de zoom


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
    
    // --- CORREÇÃO DE TELA (Adaptável) ---
    
    int monitor = GetCurrentMonitor();
    int largura_tela = GetMonitorWidth(monitor);
    int altura_tela = GetMonitorHeight(monitor);

    InitWindow(largura_tela, altura_tela, "Bomberman");
    ToggleFullscreen(); // Usa o modo tela cheia "borderless"
    
    SetTargetFPS(60);

    g_canvas_virtual = LoadRenderTexture(VIRTUAL_LARGURA, VIRTUAL_ALTURA);
    SetTextureFilter(g_canvas_virtual.texture, TEXTURE_FILTER_POINT);

    g_escala = fminf((float)largura_tela / VIRTUAL_LARGURA, (float)altura_tela / VIRTUAL_ALTURA);

    // --- CORRIGIDO: g_canvas_origem.height estava negativo ---
    // Onde vamos "recortar" do nosso canvas (a imagem inteira)
    g_canvas_origem = (Rectangle){ 0.0f, 0.0f, (float)VIRTUAL_LARGURA, (float)VIRTUAL_ALTURA };
    // Onde vamos "desenhar" o canvas na tela (centralizado)
    g_canvas_destino = (Rectangle){
        ((float)largura_tela - ((float)VIRTUAL_LARGURA * g_escala)) / 2.0f,
        ((float)altura_tela - ((float)VIRTUAL_ALTURA * g_escala)) / 2.0f,
        (float)VIRTUAL_LARGURA * g_escala,
        (float)VIRTUAL_ALTURA * g_escala
    };
    
    // --- Fim da Correção ---

    // --- Carregar Sprites ---
    g_textura_tela_inicio = LoadTexture("inicio.png"); 
    Texture2D tex_branco = LoadTexture("SpriteBranco.png");
    Texture2D tex_preto = LoadTexture("SpritePreto.png");
    Texture2D tex_azul = LoadTexture("SpriteAzul.png");
    Texture2D tex_vermelho = LoadTexture("SpriteVermelho.png");
    
    // ...

    // --- Configurar Jogadores ---
    g_jogadores[0].boneco_sprite = tex_branco;
    g_jogadores[0].cor = BOMBERMAN_BRANCO;
    g_jogadores[0].posicao = (Vector2){ TAMANHO_TILE * 1.5f, TAMANHO_TILE * 1.5f };

    g_jogadores[1].boneco_sprite = tex_preto;
    g_jogadores[1].cor = BOMBERMAN_PRETO;
    g_jogadores[1].posicao = (Vector2){ TAMANHO_TILE * (MAP_LARGURA - 1.5f), TAMANHO_TILE * 1.5f };

    g_jogadores[2].boneco_sprite = tex_azul;
    g_jogadores[2].cor = BOMBERMAN_AZUL;
    g_jogadores[2].posicao = (Vector2){ TAMANHO_TILE * 1.5f, TAMANHO_TILE * (MAP_ALTURA - 1.5f) };

    g_jogadores[3].boneco_sprite = tex_vermelho;
    g_jogadores[3].cor = BOMBERMAN_VERMELHO;
    g_jogadores[3].posicao = (Vector2){ TAMANHO_TILE * (MAP_LARGURA - 1.5f), TAMANHO_TILE * (MAP_ALTURA - 1.5f) };

    // Configurações padrão para todos
    for (int i = 0; i < 4; i++) {
        g_jogadores[i].frame_atual = 0;
        g_jogadores[i].direcao_olhando = (Vector2){0, 1}; // Olhando para baixo
        g_jogadores[i].vida = 1;
        g_jogadores[i].max_bombas = 1;
        g_jogadores[i].poder_bomba = 1;
        g_jogadores[i].bombas_plantadas = 0;
        g_jogadores[i].bot = (i == 0) ? false : true; 
    }

    g_tela_atual = TELA_MENU;
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
    // --- MUDANÇA PRINCIPAL ---
    
    // 1. Começa a desenhar no "Canvas Virtual" (600x520)
    BeginTextureMode(g_canvas_virtual);
        
        // --- CORRIGIDO: Limpa o canvas virtual de preto ---
        ClearBackground(BLACK); 

        // Desenha a tela apropriada DENTRO do canvas
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
    
    EndTextureMode();

    // 2. Começa a desenhar na tela REAL
    BeginDrawing();
    
        // Limpa a tela real (cor das barras pretas)
        ClearBackground(BLACK); 
        
        // 3. Desenha o Canvas (600x520) escalonado na tela real
        // O g_canvas_origem agora usa -VIRTUAL_ALTURA para inverter o eixo Y
        // da textura, já que DrawTexturePro desenha de baixo para cima
        // para render textures por padrão.
        DrawTexturePro(
            g_canvas_virtual.texture,
            (Rectangle){ 0.0f, 0.0f, (float)g_canvas_virtual.texture.width, (float)-g_canvas_virtual.texture.height }, // <-- AQUI INVERTE O Y
            g_canvas_destino,
            (Vector2){ 0, 0 },
            0.0f,
            WHITE
        );
        
    EndDrawing();
}

void FinalizarJogo(void) {
    // --- Descarregar Sprites ---
    
    UnloadRenderTexture(g_canvas_virtual); // Descarrega o canvas
    UnloadTexture(g_textura_tela_inicio); 

    UnloadTexture(g_jogadores[0].boneco_sprite);
    UnloadTexture(g_jogadores[1].boneco_sprite);
    UnloadTexture(g_jogadores[2].boneco_sprite);
    UnloadTexture(g_jogadores[3].boneco_sprite);

    // ...
    
    CloseWindow();
}

// 2. Funções das Telas

// --- Menu ---
void AtualizarTelaMenu(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        g_tela_atual = TELA_JOGO;
        // ReiniciarMapa(); 
    }
}
void DesenharTelaMenu(void) {
    // Desenha a imagem esticada para o canvas VIRTUAL (600x520)
    Rectangle sourceRec = { 0.0f, 0.0f, (float)g_textura_tela_inicio.width, (float)g_textura_tela_inicio.height };
    Rectangle destRec = { 0.0f, 0.0f, (float)VIRTUAL_LARGURA, (float)VIRTUAL_ALTURA };
    DrawTexturePro(g_textura_tela_inicio, sourceRec, destRec, (Vector2){0, 0}, 0.0f, WHITE);
}

// --- Jogo (Partida) ---
// (Protótipos)
void DesenharJogadores(void);
void DesenharMapa(void);
void DesenharBombas(void);

void AtualizarTelaJogo(void) {
    ProcessarTecla();
    AtualizarMovimento();
    AtualizarBombas(GetFrameTime());
}

void DesenharTelaJogo(void) {
    // Estas funções agora desenham no canvas de 600x520
    DesenharMapa();
    // DesenharItens();
    DesenharBombas();
    // DesenharExplosoes();
    DesenharJogadores();
}

// --- Tela Final ---
void AtualizarTelaFim(void) {
    if (IsKeyPressed(KEY_ENTER)) {
        g_tela_atual = TELA_MENU;
    }
}
void DesenharTelaFim(void) {
    DrawText("FIM DE JOGO", (VIRTUAL_LARGURA - MeasureText("FIM DE JOGO", 40)) / 2, 150, 40, WHITE);
    DrawText("Pressione ENTER", (VIRTUAL_LARGURA - MeasureText("Pressione ENTER", 20)) / 2, 250, 20, WHITE);
}


// 3. Funções de Lógica da Partida (Vazias)

void ReiniciarMapa(void) {}
void ProcessarTecla(void) {}
void AtualizarMovimento(void) {}
void PlantarBomba(int id_jogador) {}
void AtualizarBombas(float dt) {}
void CriarExplosao(Vector2 pos_grid, int poder) {}
void GerarItem(Vector2 pos_grid) {}


// --- Implementação das funções de desenho ---

void DesenharMapa(void) {
    // TODO
}

void DesenharBombas(void) {
    // TODO
}

void DesenharJogadores(void) {
    const int Y_OFFSET_ANIMACAO = 16; 

    for (int i = 0; i < 4; i++) {
        Jogador* jog = &g_jogadores[i];

        Rectangle sourceRec = { 0 };
        sourceRec.x = jog->frame_atual * SPRITE_LARGURA; 
        sourceRec.y = Y_OFFSET_ANIMACAO;                 
        sourceRec.width = SPRITE_LARGURA;
        sourceRec.height = SPRITE_ALTURA;

        Vector2 destPos = {
            jog->posicao.x - (TAMANHO_TILE / 2) + 4.0f,
            jog->posicao.y - (TAMANHO_TILE / 2) - 4.0f
        };

        DrawTextureRec(jog->boneco_sprite, sourceRec, destPos, WHITE);
    }
}
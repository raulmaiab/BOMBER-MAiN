#include "raylib.h"
#include "derrota.h" 
#include <stdbool.h>
#include <math.h> 
#include <stddef.h>  // Para fmod, NULL

// --- Constantes ---
#define TEXTO_TITULO        "FIM DE JOGO"
#define TEXTO_SUBTITULO     "Pressione ENTER para voltar ao menu"
#define CAMINHO_FONTE       "assets/font.ttf"  // (O mesmo do inicio.c)

// --- FUNÇÕES GRÁFICAS (Traduzidas) ---

static void DesenharFundoNeon(int largura, int altura) {
    // Gradiente de fundo (topo mais escuro → base um pouco mais clara)
    for (int y = 0; y < altura; y++) {
        float t = (float)y / (float)altura;
        Color c = {
            (unsigned char)(6 + (int)(12  * t)),  // R
            (unsigned char)(10 + (int)(16 * t)),  // G
            (unsigned char)(18 + (int)(24 * t)),  // B
            255
        };
        DrawLine(0, y, largura, y, c);
    }
    // “Linhas holográficas” verticais semi-transparentes
    Color linhaCiano = (Color){ 0, 200, 255, 40 };
    for (int i = 0; i < 24; i++) {
        int x = (i * 53) % largura;
        DrawRectangle(x, 0, 2, altura, linhaCiano);
    }
}

static void DesenharTextoBrilho(Font fonte, const char* texto, Vector2 pos, float tamanho, float espacamento, Color base, Color brilho) {
    // Leve brilho desenhando o texto 4x ao redor
    const int o = 2;
    DrawTextEx(fonte, texto, (Vector2){ pos.x - o, pos.y }, tamanho, espacamento, brilho);
    DrawTextEx(fonte, texto, (Vector2){ pos.x + o, pos.y }, tamanho, espacamento, brilho);
    DrawTextEx(fonte, texto, (Vector2){ pos.x, pos.y - o }, tamanho, espacamento, brilho);
    DrawTextEx(fonte, texto, (Vector2){ pos.x, pos.y + o }, tamanho, espacamento, brilho);
    // Texto principal
    DrawTextEx(fonte, texto, pos, tamanho, espacamento, base);
}

// --- Função Principal da Tela ---

bool ExecutarTelaDerrota(void)
{
    // Carrega fonte opcional
    Font fonte = GetFontDefault();
    bool temFonte = false;
    
    if (FileExists(CAMINHO_FONTE)) {
        fonte = LoadFontEx(CAMINHO_FONTE, 64, NULL, 0);
        if (fonte.baseSize > 0) {
            temFonte = true;
        }
    }

    // Loop da tela
    bool voltarAoMenu = false;
    
    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0)
    {
        // INPUT
        if (IsKeyPressed(KEY_F11)) {
            ToggleFullscreen();
        }
        if (IsKeyPressed(KEY_ESCAPE)) { 
            voltarAoMenu = false; 
            break; 
        }
        
        // --- Usa IsKeyReleased para evitar "input bleed" para o menu ---
        if (IsKeyReleased(KEY_ENTER))  { 
            voltarAoMenu = true;  
            break; 
        }
        // --- FIM ---

        // LAYOUT
        const int larguraTela = GetScreenWidth();
        const int alturaTela = GetScreenHeight();

        BeginDrawing();
        {
            ClearBackground(BLACK);
            DesenharFundoNeon(larguraTela, alturaTela); // O fundo neon

            // TEXTOS
            const float tamanhoTitulo = 100.0f; // Tamanho do Fim de Jogo
            const float tamanhoSubtitulo   = 36.0f;
            const float espacamento   = 2.0f;

            // Medidas para centralizar
            Vector2 tamanhoT = MeasureTextEx(fonte, TEXTO_TITULO, tamanhoTitulo, espacamento);
            Vector2 tamanhoS = MeasureTextEx(fonte, TEXTO_SUBTITULO, tamanhoSubtitulo, espacamento);

            Vector2 posT = { (larguraTela - tamanhoT.x)/2.0f, alturaTela*0.35f }; // Posição do Fim de Jogo
            Vector2 posS = { (larguraTela - tamanhoS.x)/2.0f, alturaTela*0.60f }; // Posição do Subtexto

            // --- Desenha "FIM DE JOGO" em Vermelho ---
            DesenharTextoBrilho(fonte, TEXTO_TITULO, posT, tamanhoTitulo, espacamento,
                         (Color){255, 0, 0, 255},   // Base Vermelha
                         (Color){150, 0, 0, 120 }); // Brilho Vermelho Escuro

            // “Pressione ENTER” piscando
            double piscar = fmod(GetTime(), 0.8);
            if (piscar < 0.4) {
                DesenharTextoBrilho(fonte, TEXTO_SUBTITULO, posS, tamanhoSubtitulo, espacamento,
                             (Color){140,230,255,255}, (Color){ 10,120,200,110 });
            }

            // Dica no canto
            const char* dica = "[ESC] Sair";
            Vector2 tamanhoH = MeasureTextEx(fonte, dica, 18.0f, 1.0f);
            DrawTextEx(fonte, dica, (Vector2){ larguraTela - tamanhoH.x - 12, alturaTela - tamanhoH.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
        }
        EndDrawing();
    }

    // Descarrega fonte
    if (temFonte) {
        UnloadFont(fonte);
    }

    // Retorna true (para voltar ao menu) ou false (para fechar o jogo)
    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    if (WindowShouldClose() == 0 && voltarAoMenu == true) {
        return true;
    } else {
        return false;
    }
}
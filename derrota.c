#include "raylib.h"
#include "derrota.h" // Inclui o .h que acabamos de criar
#include <stdbool.h>
#include <math.h> 
#include <stddef.h>  // Para fmod, sin, NULL

// --- Constantes ---
#define TITLE_TEXT      "GAME OVER"
#define SUBTEXT_TEXT    "Press ENTER to back to menu"
#define FONT_PATH       "assets/font.ttf"  // (O mesmo do inicio.c)

// --- FUNÇÕES GRÁFICAS (Copiadas do inicio.c para manter o tema) ---

static void DrawNeonBackground(int w, int h) {
    // Gradiente de fundo (topo mais escuro → base um pouco mais clara)
    for (int y = 0; y < h; y++) {
        float t = (float)y / (float)h;
        Color c = {
            (unsigned char)(6 + (int)(12  * t)),  // R
            (unsigned char)(10 + (int)(16 * t)),  // G
            (unsigned char)(18 + (int)(24 * t)),  // B
            255
        };
        DrawLine(0, y, w, y, c);
    }
    // “Linhas holográficas” verticais semi-transparentes
    Color cyanLine = (Color){ 0, 200, 255, 40 };
    for (int i = 0; i < 24; i++) {
        int x = (i * 53) % w;
        DrawRectangle(x, 0, 2, h, cyanLine);
    }
}

static void DrawTextGlow(Font font, const char* text, Vector2 pos, float size, float spacing, Color base, Color glow) {
    // Leve glow desenhando o texto 4x ao redor
    const int o = 2;
    DrawTextEx(font, text, (Vector2){ pos.x - o, pos.y }, size, spacing, glow);
    DrawTextEx(font, text, (Vector2){ pos.x + o, pos.y }, size, spacing, glow);
    DrawTextEx(font, text, (Vector2){ pos.x, pos.y - o }, size, spacing, glow);
    DrawTextEx(font, text, (Vector2){ pos.x, pos.y + o }, size, spacing, glow);
    // Texto principal
    DrawTextEx(font, text, pos, size, spacing, base);
}

// --- Função Principal da Tela ---

bool ExecutarTelaDerrota(void)
{
    // Carrega fonte opcional
    Font font = GetFontDefault();
    bool hasFont = false;
    if (FileExists(FONT_PATH)) {
        font = LoadFontEx(FONT_PATH, 64, NULL, 0);
        if (font.baseSize > 0) hasFont = true;
    }

    // Loop da tela
    bool backToMenu = false;
    while (!WindowShouldClose())
    {
        // INPUT
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        if (IsKeyPressed(KEY_ESCAPE)) { backToMenu = false; break; }
        
        // --- USA IsKeyReleased para evitar "input bleed" para o menu ---
        if (IsKeyReleased(KEY_ENTER))  { backToMenu = true;  break; }
        // --- FIM ---

        // LAYOUT
        const int sw = GetScreenWidth();
        const int sh = GetScreenHeight();

        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawNeonBackground(sw, sh); // O fundo neon

            // TEXTOS
            const float titleSize = 100.0f; // Tamanho do Game Over
            const float subSize   = 36.0f;
            const float spacing   = 2.0f;

            // Medidas para centralizar
            Vector2 tSize = MeasureTextEx(font, TITLE_TEXT, titleSize, spacing);
            Vector2 sSize = MeasureTextEx(font, SUBTEXT_TEXT, subSize, spacing);

            Vector2 tPos = { (sw - tSize.x)/2.0f, sh*0.35f }; // Posição do Game Over
            Vector2 sPos = { (sw - sSize.x)/2.0f, sh*0.60f }; // Posição do Subtexto

            // --- ALTERADO: Desenha "GAME OVER" em Vermelho ---
            DrawTextGlow(font, TITLE_TEXT, tPos, titleSize, spacing,
                         (Color){255, 0, 0, 255},   // Base Vermelha
                         (Color){150, 0, 0, 120 }); // Glow Vermelho Escuro

            // “Press ENTER” piscando (igual ao inicio.c)
            double blink = fmod(GetTime(), 0.8);
            if (blink < 0.4) {
                DrawTextGlow(font, SUBTEXT_TEXT, sPos, subSize, spacing,
                             (Color){140,230,255,255}, (Color){ 10,120,200,110 });
            }

            // Dica no canto
            const char* hint = "[ESC] Quit";
            Vector2 hSize = MeasureTextEx(font, hint, 18.0f, 1.0f);
            DrawTextEx(font, hint, (Vector2){ sw - hSize.x - 12, sh - hSize.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
        }
        EndDrawing();
    }

    // Unload
    if (hasFont) UnloadFont(font);

    // Retorna true (para voltar ao menu) ou false (para fechar o jogo)
    return backToMenu && !WindowShouldClose();
}
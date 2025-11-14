#include "raylib.h"
#include "vitoria.h" // Inclui o .h que acabamos de criar
#include <stdbool.h>
#include <math.h> 
#include <stddef.h>  // Para fmod, sin, NULL

// --- Constantes ---
#define TITLE_TEXT      "YOU WON"
#define SUBTEXT_TEXT    "Press ENTER to back to menu"
#define FONT_PATH       "assets/font.ttf"  // (O mesmo do inicio.c)

// --- FUNÇÕES GRÁFICAS (Copiadas do inicio.c/derrota.c para manter o tema) ---

static void DrawNeonBackground(int w, int h) {
    for (int y = 0; y < h; y++) {
        float t = (float)y / (float)h;
        Color c = { (unsigned char)(6 + (int)(12 * t)), (unsigned char)(10 + (int)(16 * t)), (unsigned char)(18 + (int)(24 * t)), 255 };
        DrawLine(0, y, w, y, c);
    }
    Color cyanLine = (Color){ 0, 200, 255, 40 };
    for (int i = 0; i < 24; i++) {
        int x = (i * 53) % w;
        DrawRectangle(x, 0, 2, h, cyanLine);
    }
}

static void DrawTextGlow(Font font, const char* text, Vector2 pos, float size, float spacing, Color base, Color glow) {
    const int o = 2;
    DrawTextEx(font, text, (Vector2){ pos.x - o, pos.y }, size, spacing, glow);
    DrawTextEx(font, text, (Vector2){ pos.x + o, pos.y }, size, spacing, glow);
    DrawTextEx(font, text, (Vector2){ pos.x, pos.y - o }, size, spacing, glow);
    DrawTextEx(font, text, (Vector2){ pos.x, pos.y + o }, size, spacing, glow);
    DrawTextEx(font, text, pos, size, spacing, base);
}

// --- Função Principal da Tela ---

bool ExecutarTelaVitoria(void)
{
    Font font = GetFontDefault();
    bool hasFont = false;
    if (FileExists(FONT_PATH)) {
        font = LoadFontEx(FONT_PATH, 64, NULL, 0);
        if (font.baseSize > 0) hasFont = true;
    }

    bool backToMenu = false;
    while (!WindowShouldClose())
    {
        // INPUT
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        if (IsKeyPressed(KEY_ESCAPE)) { backToMenu = false; break; }
        if (IsKeyReleased(KEY_ENTER)) { backToMenu = true;  break; } // Evita "input bleed"

        // LAYOUT
        const int sw = GetScreenWidth();
        const int sh = GetScreenHeight();

        BeginDrawing();
        {
            ClearBackground(BLACK);
            DrawNeonBackground(sw, sh); 

            const float titleSize = 100.0f;
            const float subSize   = 36.0f;
            const float spacing   = 2.0f;

            Vector2 tSize = MeasureTextEx(font, TITLE_TEXT, titleSize, spacing);
            Vector2 sSize = MeasureTextEx(font, SUBTEXT_TEXT, subSize, spacing);

            Vector2 tPos = { (sw - tSize.x)/2.0f, sh*0.35f }; 
            Vector2 sPos = { (sw - sSize.x)/2.0f, sh*0.60f }; 

            // --- ALTERADO: Desenha "YOU WON" em Verde ---
            DrawTextGlow(font, TITLE_TEXT, tPos, titleSize, spacing,
                         (Color){0, 255, 128, 255},   // Base Verde Neon
                         (Color){0, 150, 70, 120 }); // Glow Verde Escuro

            // “Press ENTER” piscando (azul/ciano)
            double blink = fmod(GetTime(), 0.8);
            if (blink < 0.4) {
                DrawTextGlow(font, SUBTEXT_TEXT, sPos, subSize, spacing,
                             (Color){140,230,255,255}, (Color){ 10,120,200,110 });
            }
            
            const char* hint = "[ESC] Quit";
            Vector2 hSize = MeasureTextEx(font, hint, 18.0f, 1.0f);
            DrawTextEx(font, hint, (Vector2){ sw - hSize.x - 12, sh - hSize.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
        }
        EndDrawing();
    }

    if (hasFont) UnloadFont(font);
    
    return backToMenu && !WindowShouldClose();
}
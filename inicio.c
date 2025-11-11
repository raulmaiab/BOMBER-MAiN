#include "raylib.h"
#include "inicio.h"
#include <stdbool.h>
#include <math.h> 
#include <stddef.h>  // para fmod, sin

// Ajustes visuais
#define TITLE_TEXT      "BomberMAIN"
#define SUBTEXT_TEXT    "Press ENTER"
#define BG_IMAGE_PATH   "inicio.png"       // mantém o seu nome/raiz atual
#define FONT_PATH       "assets/font.ttf"  // opcional; se não existir, usa fonte default

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

bool ExecutarTelaInicio(void)
{
    // Carrega opcionalmente a imagem de fundo
    Texture2D bg = {0};
    bool hasBg = false;
    if (FileExists(BG_IMAGE_PATH)) {
        bg = LoadTexture(BG_IMAGE_PATH);
        hasBg = (bg.id != 0);
    }

    // Carrega fonte opcional (para título/subtexto); se falhar, usa default
    Font font = GetFontDefault();
    bool hasFont = false;
    if (FileExists(FONT_PATH)) {
        font = LoadFontEx(FONT_PATH, 64, NULL, 0);
        if (font.baseSize > 0) hasFont = true;
    }

    // Loop da tela de início
    bool startGame = false;
    while (!WindowShouldClose())
    {
        // INPUT
        if (IsKeyPressed(KEY_F11)) ToggleFullscreen();
        if (IsKeyPressed(KEY_ESCAPE)) { startGame = false; break; }
        if (IsKeyPressed(KEY_ENTER))  { startGame = true;  break; }

        // LAYOUT
        const int sw = GetScreenWidth();
        const int sh = GetScreenHeight();

        BeginDrawing();
        {
            ClearBackground(BLACK);

            // Fundo: imagem (se existir) ajustada para preencher, senão fundo neon
            if (hasBg) {
                // letterbox/cover simples mantendo proporção
                float scale = fminf((float)sw / bg.width, (float)sh / bg.height);
                int w = (int)(bg.width * scale);
                int h = (int)(bg.height * scale);
                int x = (sw - w)/2;
                int y = (sh - h)/2;
                DrawTexturePro(
                    bg,
                    (Rectangle){ 0, 0, (float)bg.width, (float)bg.height },
                    (Rectangle){ (float)x, (float)y, (float)w, (float)h },
                    (Vector2){ 0, 0 }, 0.0f, WHITE
                );
                // leve vinheta
                DrawRectangleGradientV(0, 0, sw, sh, (Color){0,0,0,60}, (Color){0,0,0,140});
            } else {
                DrawNeonBackground(sw, sh);
            }

            // TEXTOS (título + subtexto)
            const float titleSize = 92.0f;
            const float subSize   = 36.0f;
            const float spacing   = 2.0f;

            // Medidas para centralizar
            Vector2 tSize = MeasureTextEx(font, TITLE_TEXT, titleSize, spacing);
            Vector2 sSize = MeasureTextEx(font, SUBTEXT_TEXT, subSize, spacing);

            Vector2 tPos = { (sw - tSize.x)/2.0f, sh*0.20f };
            Vector2 sPos = { (sw - sSize.x)/2.0f, sh*0.60f };

            // Título com glow
            DrawTextGlow(font, TITLE_TEXT, tPos, titleSize, spacing,
                         (Color){150,240,255,255}, (Color){ 20,140,180,120 });

            // “Press ENTER” piscando
            double blink = fmod(GetTime(), 0.8);
            if (blink < 0.4) {
                DrawTextGlow(font, SUBTEXT_TEXT, sPos, subSize, spacing,
                             (Color){140,230,255,255}, (Color){ 10,120,200,110 });
            }

            // Dica no canto
            const char* hint = "[ESC] Quit   |   [F11] Fullscreen";
            Vector2 hSize = MeasureTextEx(font, hint, 18.0f, 1.0f);
            DrawTextEx(font, hint, (Vector2){ sw - hSize.x - 12, sh - hSize.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
        }
        EndDrawing();
    }

    // Unload recursos
    if (hasBg) UnloadTexture(bg);
    if (hasFont) UnloadFont(font);

    // Se fechou a janela pelo X, WindowShouldClose() ficou true → aqui retornamos false
    return startGame && !WindowShouldClose();
}

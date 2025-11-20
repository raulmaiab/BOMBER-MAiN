#include "raylib.h"
#include "inicio.h"
#include <stdbool.h>
#include <math.h>
#include <stddef.h>

#define TITLE_TEXT      "BomberMAIN"
#define SUBTEXT_TEXT    "Press ENTER"
#define BG_IMAGE_PATH   "inicio.png"      
#define FONT_PATH       "assets/font.ttf" 


static void LinhasNeonFundo(int largura, int altura) {
   for (int coord_vertical = 0; coord_vertical < altura; coord_vertical++) {
       float progresso = (float)coord_vertical / (float)altura;
       Color cor = {
           (unsigned char)(6 + (int)(12  * progresso)), 
           (unsigned char)(10 + (int)(16 * progresso)),
           (unsigned char)(18 + (int)(24 * progresso)), 
           255
       };
       DrawLine(0, coord_vertical, largura, coord_vertical, cor);
   }


   Color linhasciano = (Color){ 0, 200, 255, 40 };
   for (int linha = 0; linha < 28; linha++) {
       int coord_horizontal = (linha * 53) % largura;
       DrawRectangle(coord_horizontal, 0, 2, altura, linhasciano);
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


bool ExecutarTelaInicio(void)
{
   Texture2D bg = {0};
   bool hasBg = false;
   if (FileExists(BG_IMAGE_PATH)) {
       bg = LoadTexture(BG_IMAGE_PATH);
       hasBg = (bg.id != 0);
   }


   Font font = GetFontDefault();
   bool hasFont = false;
   if (FileExists(FONT_PATH)) {
       font = LoadFontEx(FONT_PATH, 64, NULL, 0);
       if (font.baseSize > 0) {
           hasFont = true;
       }
   }


   bool startGame = false;
   while (WindowShouldClose() == 0)
   {
       if (IsKeyPressed(KEY_F11)) {
           ToggleFullscreen();
       }
       if (IsKeyPressed(KEY_ESCAPE)) {
           startGame = false; break;
       }
       if (IsKeyReleased(KEY_ENTER)) {
           startGame = true;  break;
       }
       const int largura_tela = GetScreenWidth();
       const int altura_tela = GetScreenHeight();


       BeginDrawing();
       {
           ClearBackground(BLACK);


           if (hasBg) {
               float scale = fminf((float)largura_tela / bg.width, (float)altura_tela / bg.height);
               int w = (int)(bg.width * scale);
               int h = (int)(bg.height * scale);
               int x = (largura_tela - w)/2;
               int y = (altura_tela - h)/2;
               DrawTexturePro(
                   bg,
                   (Rectangle){ 0, 0, (float)bg.width, (float)bg.height },
                   (Rectangle){ (float)x, (float)y, (float)w, (float)h },
                   (Vector2){ 0, 0 }, 0.0f, WHITE
               );
               DrawRectangleGradientV(0, 0, largura_tela, altura_tela, (Color){0,0,0,60}, (Color){0,0,0,140});
           } else {
               LinhasNeonFundo(largura_tela, altura_tela);
           }


           const float titleSize = 92.0f;
           const float subSize   = 36.0f;
           const float spacing   = 2.0f;


           Vector2 tSize = MeasureTextEx(font, TITLE_TEXT, titleSize, spacing);
           Vector2 sSize = MeasureTextEx(font, SUBTEXT_TEXT, subSize, spacing);


           Vector2 tPos = { (largura_tela - tSize.x)/2.0f, altura_tela*0.20f };
           Vector2 sPos = { (largura_tela - sSize.x)/2.0f, altura_tela*0.60f };


           DrawTextGlow(font, TITLE_TEXT, tPos, titleSize, spacing,
                        (Color){150,240,255,255}, (Color){ 20,140,180,120 });


           double blink = fmod(GetTime(), 0.8);
           if (blink < 0.4) {
               DrawTextGlow(font, SUBTEXT_TEXT, sPos, subSize, spacing,
                            (Color){140,230,255,255}, (Color){ 10,120,200,110 });
           }


           const char* hint = "[ESC] Quit   |   [F11] Fullscreen";
           Vector2 hSize = MeasureTextEx(font, hint, 18.0f, 1.0f);
           DrawTextEx(font, hint, (Vector2){ largura_tela - hSize.x - 12, altura_tela - hSize.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
       }
       EndDrawing();
   }


   if (hasBg) {
       UnloadTexture(bg);
   }
   if (hasFont){
       UnloadFont(font);
   }


   return startGame && WindowShouldClose() == 0;
}

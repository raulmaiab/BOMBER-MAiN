#include "raylib.h"
#include "vitoria.h"
#include <stdbool.h>
#include <math.h>
#include <stddef.h> 

#define TEXTO_TITULO        "VOCÊ VENCEU"
#define TEXTO_SUBTITULO     "Pressione ENTER para voltar ao menu"
#define CAMINHO_FONTE       "assets/font.ttf"

//Funções gráficas
static void DesenharFundoNeon(int largura, int altura) {
   for (int y = 0; y < altura; y++) {
       float t = (float)y / (float)altura;
       Color c = { (unsigned char)(6 + (int)(12 * t)), (unsigned char)(10 + (int)(16 * t)), (unsigned char)(18 + (int)(24 * t)), 255 };
       DrawLine(0, y, largura, y, c);
   }
   Color linhaCiano = (Color){ 0, 200, 255, 40 };
   for (int i = 0; i < 28; i++) {
       int x = (i * 53) % largura;
       DrawRectangle(x, 0, 2, altura, linhaCiano);
   }
}

static void DesenharTextoBrilho(Font fonte, const char* texto, Vector2 pos, float tamanho, float espacamento, Color base, Color brilho) {
   const int o = 2;
   DrawTextEx(fonte, texto, (Vector2){ pos.x - o, pos.y }, tamanho, espacamento, brilho);
   DrawTextEx(fonte, texto, (Vector2){ pos.x + o, pos.y }, tamanho, espacamento, brilho);
   DrawTextEx(fonte, texto, (Vector2){ pos.x, pos.y - o }, tamanho, espacamento, brilho);
   DrawTextEx(fonte, texto, (Vector2){ pos.x, pos.y + o }, tamanho, espacamento, brilho);
   DrawTextEx(fonte, texto, pos, tamanho, espacamento, base);
}

//Função principal
bool ExecutarTelaVitoria(void)
{
   Font fonte = GetFontDefault();
   bool temFonte = false;
  
   if (FileExists(CAMINHO_FONTE)) {
       fonte = LoadFontEx(CAMINHO_FONTE, 64, NULL, 0);
       if (fonte.baseSize > 0) {
           temFonte = true;
       }
   }

   bool voltarAoMenu = false;
  
   while (WindowShouldClose() == 0)
   {
       //Atualizar ações
       if (IsKeyPressed(KEY_F11)) {
           ToggleFullscreen();
       }
       if (IsKeyPressed(KEY_ESCAPE)) {
           voltarAoMenu = false;
           break;
       }
       if (IsKeyReleased(KEY_ENTER)) {
           voltarAoMenu = true; 
           break;
       }

       //Layout
       const int larguraTela = GetScreenWidth();
       const int alturaTela = GetScreenHeight();

       BeginDrawing();
       {
           ClearBackground(BLACK);
           DesenharFundoNeon(larguraTela, alturaTela);

           const float tamanhoTitulo = 100.0f;
           const float tamanhoSubtitulo   = 36.0f;
           const float espacamento   = 2.0f;

           Vector2 tamanhoT = MeasureTextEx(fonte, TEXTO_TITULO, tamanhoTitulo, espacamento);
           Vector2 tamanhoS = MeasureTextEx(fonte, TEXTO_SUBTITULO, tamanhoSubtitulo, espacamento);

           Vector2 posT = { (larguraTela - tamanhoT.x)/2.0f, alturaTela*0.35f };
           Vector2 posS = { (larguraTela - tamanhoS.x)/2.0f, alturaTela*0.60f };

           //"Você Venceu" em verde
           DesenharTextoBrilho(fonte, TEXTO_TITULO, posT, tamanhoTitulo, espacamento, (Color){0, 255, 128, 255}, (Color){0, 150, 70, 120 });

           //“Pressione ENTER” piscando azul
           double piscar = fmod(GetTime(), 0.8);
           if (piscar < 0.4) {
               DesenharTextoBrilho(fonte, TEXTO_SUBTITULO, posS, tamanhoSubtitulo, espacamento, (Color){140,230,255,255}, (Color){ 10,120,200,110 });
           }
          
           const char* dica = "[ESC] Sair";
           Vector2 tamanhoH = MeasureTextEx(fonte, dica, 18.0f, 1.0f);
           DrawTextEx(fonte, dica, (Vector2){ larguraTela - tamanhoH.x - 12, alturaTela - tamanhoH.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
       }
       EndDrawing();
   }

   if (temFonte) {
       UnloadFont(fonte);
   }
   //Enter == true
   if (WindowShouldClose() == 0 && voltarAoMenu == true) {
       return true;
   } else {
       return false;
   }
}

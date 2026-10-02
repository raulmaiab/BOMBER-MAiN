#include "vitoria_battle.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

const int TAMANHO_FONTE = 70;

//Função pra pegar nome do sprite
static const char* ObterNomeCor(const char* nomeSprite) {
   if (strcmp(nomeSprite, "SpriteBranco") == 0) {
       return "BRANCO";
   } else if (strcmp(nomeSprite, "SpriteVermelho") == 0) {
       return "VERMELHO";
   } else if (strcmp(nomeSprite, "SpriteAzul") == 0) {
       return "AZUL";
   } else if (strcmp(nomeSprite, "SpritePreto") == 0) {
       return "PRETO";
   }
   //Caso não encontre, retorna o nome original.
   return TextToUpper(nomeSprite);
}

void ExecutarTelaVitoriaBattle(const char* nomeSpriteVencedor)
{
   //Obtém o nome da cor
   const char* corVencedor = ObterNomeCor(nomeSpriteVencedor);

   char tituloDinamico[256];
  
   //Constrói a string do título (ex: BRANCO VENCEU!)
   sprintf(tituloDinamico, "%s VENCEU!", corVencedor);
  
   //Calcula o centro da tela para o texto
   int larguraTela = GetScreenWidth();
   int alturaTela = GetScreenHeight();
  
   Vector2 tamanhoTitulo = MeasureTextEx(GetFontDefault(), tituloDinamico, TAMANHO_FONTE, 2);
   int xPos = (larguraTela - tamanhoTitulo.x) / 2;
   int yPos = alturaTela / 2 - TAMANHO_FONTE;
  
   //Define a cor de exibição
   Color corExibicao = WHITE;
  
   if (strcmp(corVencedor, "BRANCO") == 0) {
       corExibicao = LIGHTGRAY;
   } else if (strcmp(corVencedor, "VERMELHO") == 0) {
       corExibicao = RED;
   } else if (strcmp(corVencedor, "AZUL") == 0) {
       corExibicao = SKYBLUE;
   } else if (strcmp(corVencedor, "PRETO") == 0) {
       corExibicao = DARKGRAY;
   }

   while (WindowShouldClose() == 0)
   {
       if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)) {
           break;
       }

       BeginDrawing();
       {
           ClearBackground(BLACK);
          
           //Desenha o título dinâmico do vencedor
           DrawTextEx(GetFontDefault(), tituloDinamico, (Vector2){(float)xPos, (float)yPos}, (float)TAMANHO_FONTE, 2, corExibicao);
          
           //Desenha a dica pra voltar
           const char* textoInstrucao = "Pressione ESPAÇO para voltar ao Menu Principal...";
           DrawText(textoInstrucao, (larguraTela - MeasureText(textoInstrucao, 20)) / 2, alturaTela - 50, 20, RAYWHITE);
       }
       EndDrawing();
   }
}
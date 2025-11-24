#include "raylib.h"
#include "inicio.h"
#include <stdbool.h>
#include <math.h>
#include <stddef.h>
#include <string.h>


#define TEXTO_TITULO        "BomberMAIN"
#define TEXTO_SUBTITULO     "Pressione ENTER"
#define CAMINHO_IMAGEM_FUNDO "inicio.png"     
#define CAMINHO_FONTE       "assets/font.ttf"

// Desenhando a tela de início
static void LinhasNeonFundo(int largura_tela, int altura_tela) {
  for (int coordenada_vertical = 0; coordenada_vertical < altura_tela; coordenada_vertical++) {
      float progresso_vertical = (float)coordenada_vertical / (float)altura_tela;
      Color cor_gradiente = {
          (unsigned char)(6 + (int)(12  * progresso_vertical)),
          (unsigned char)(10 + (int)(16 * progresso_vertical)),
          (unsigned char)(18 + (int)(24 * progresso_vertical)),
          255
      };
      DrawLine(0, coordenada_vertical, largura_tela, coordenada_vertical, cor_gradiente);
  }

  // Linhas verticais ciano (efeito de grade)
  Color cor_linhas_ciano = (Color){ 0, 200, 255, 40 };
  for (int indice_linha = 0; indice_linha < 28; indice_linha++) {
      int coordenada_horizontal = (indice_linha * 53) % largura_tela;
      DrawRectangle(coordenada_horizontal, 0, 2, altura_tela, cor_linhas_ciano);
  }
}

static void DesenharTextoBrilhante(Font fonte, const char* frase, Vector2 posicao_inicial, float tamanho_fonte, float espacamento, Color cor_base, Color cor_brilho) {
  const int deslocamento_brilho = 2;
  
  // Desenha o brilho (sombra)
  DrawTextEx(fonte, frase, (Vector2){ posicao_inicial.x - deslocamento_brilho, posicao_inicial.y }, tamanho_fonte, espacamento, cor_brilho);
  DrawTextEx(fonte, frase, (Vector2){ posicao_inicial.x + deslocamento_brilho, posicao_inicial.y }, tamanho_fonte, espacamento, cor_brilho);
  DrawTextEx(fonte, frase, (Vector2){ posicao_inicial.x, posicao_inicial.y - deslocamento_brilho }, tamanho_fonte, espacamento, cor_brilho);
  DrawTextEx(fonte, frase, (Vector2){ posicao_inicial.x, posicao_inicial.y + deslocamento_brilho }, tamanho_fonte, espacamento, cor_brilho);
  
  // Desenha o texto principal por cima
  DrawTextEx(fonte, frase, posicao_inicial, tamanho_fonte, espacamento, cor_base);
}

// def principal

bool ExecutarTelaInicio(void) {

  // tenta carregar imagem de fundo
  Texture2D textura_fundo = {0};

  // Carregando a fonte
  Font fonte_principal = GetFontDefault();

  // Loop da tela de início
  bool iniciar_jogo = false;
  while (WindowShouldClose() == 0)
  {
      
      if (IsKeyPressed(KEY_ESCAPE)) {
          iniciar_jogo = false; 
          break;
      }
      
      if (IsKeyReleased(KEY_ENTER)) {
          iniciar_jogo = true;  
          break;
      }
      
      const int largura_tela = GetScreenWidth();
      const int altura_tela = GetScreenHeight();


      BeginDrawing();
      {
          ClearBackground(BLACK);
          LinhasNeonFundo(largura_tela, altura_tela);
        
          const float tamanho_titulo = 92.0f;
          const float tamanho_subtitulo = 36.0f;
          const float espacamento_texto = 2.0f;

          //Medir o texto
          Vector2 tamanho_titulo_encontrado = MeasureTextEx(fonte_principal, TEXTO_TITULO, tamanho_titulo, espacamento_texto);
          Vector2 tamanho_subtitulo_encontrado = MeasureTextEx(fonte_principal, TEXTO_SUBTITULO, tamanho_subtitulo, espacamento_texto);

          //Calcular a posição central
          Vector2 posicao_titulo = { (largura_tela - tamanho_titulo_encontrado.x)/2.0f, altura_tela*0.20f };
          Vector2 posicao_subtitulo = { (largura_tela - tamanho_subtitulo_encontrado.x)/2.0f, altura_tela*0.60f };

          //Desenha o TÍTULO
          DesenharTextoBrilhante(fonte_principal, TEXTO_TITULO, posicao_titulo, tamanho_titulo, espacamento_texto,
                       (Color){150,240,255,255}, (Color){ 20,140,180,120 });

          // Lógica do efeito de piscar
          double tempo_piscando = fmod(GetTime(), 0.8);
          if (tempo_piscando < 0.4) {
              // Desenha o SUBTÍTULO
              DesenharTextoBrilhante(fonte_principal, TEXTO_SUBTITULO, posicao_subtitulo, tamanho_subtitulo, espacamento_texto,
                           (Color){140,230,255,255}, (Color){ 10,120,200,110 });
          }

          // Desenha a dica de teclas
          const char* dica_teclas = "[ESC] Sair";
          Vector2 tamanho_dica = MeasureTextEx(fonte_principal, dica_teclas, 18.0f, 1.0f);
          DrawTextEx(fonte_principal, dica_teclas, (Vector2){ largura_tela - tamanho_dica.x - 12, altura_tela - tamanho_dica.y - 8 }, 18.0f, 1.0f, (Color){ 200, 220, 235, 200 });
      }
      EndDrawing();
  }


  // Retorna true se iniciou o jogo E a janela NÃO foi fechada (== 0)
  return iniciar_jogo && WindowShouldClose() == 0;
}
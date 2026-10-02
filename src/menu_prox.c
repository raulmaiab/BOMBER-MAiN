#include "menu_prox.h"
#include "raylib.h"
#include <stdio.h>

//Cores do tema do menu
#define COR_BOTAO_NORMAL CLITERAL(Color){ 150, 150, 150, 255 }
#define COR_BOTAO_HOVER CLITERAL(Color){ 255, 255, 255, 255 }
#define COR_SUCESSO GREEN
#define COR_TITULO RAYWHITE
#define COR_FUNDO CLITERAL(Color){ 20, 20, 20, 255 }

//Lógica do próximo nível (passar de mapa)
AcaoOpcoes ExecutarMenuProximoNivel(InfoNivel info)
{
   AcaoOpcoes acao = OPCOES_ACAO_NENHUMA;
  
   //Posição central do botão START
   int xBotao = GetScreenWidth() / 2;
   int yBotao = (GetScreenHeight() / 2) + 150;
  
   //Texto do botão
   const char* textoBotao = "INICIAR PRÓXIMO NÍVEL (ENTER)";
   int tamanhoFonteBotao = 40;
   int larguraBotao = MeasureText(textoBotao, tamanhoFonteBotao);
  
   //Área de colisão do botão (usada para hover e clique)
   Rectangle limitesBotao = {
       (float)xBotao - (float)larguraBotao / 2,
       (float)yBotao - (float)tamanhoFonteBotao / 2,
       (float)larguraBotao,
       (float)tamanhoFonteBotao
   };
  
   //Textos de dica
   char nivelConcluido[64];
   sprintf(nivelConcluido, "NÍVEL %d: CONCLUÍDO", info.nivelAtual + 1);
  
   char proximaFase[64];
   sprintf(proximaFase, "PRÓXIMA FASE: %s", info.proximoMapa);
  
   while (WindowShouldClose() == 0)
   {
       Vector2 pontoMouse = GetMousePosition();
       bool mouseSobreBotao = CheckCollisionPointRec(pontoMouse, limitesBotao);

       //Atualizar ações
       if (IsKeyPressed(KEY_ESCAPE)) {
           acao = OPCOES_ACAO_MENU_PRINCIPAL;
           break;
       }

       //Enter
       if (IsKeyPressed(KEY_ENTER) || (mouseSobreBotao && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
       {
           acao = OPCOES_ACAO_NENHUMA;
           break;
       }
     
       BeginDrawing();
       {
           ClearBackground(COR_FUNDO);
          
           //Título de Sucesso
           DrawText("VOCÊ VENCEU", (GetScreenWidth() / 2) - MeasureText("VOCÊ VENCEU", 80) / 2, 100,80, COR_SUCESSO);

           //Nível Concluído
           DrawText(nivelConcluido, (GetScreenWidth() / 2) - MeasureText(nivelConcluido, 35) / 2, 280, 35, COR_TITULO);

           //Próxima Fase
           DrawText(proximaFase, (GetScreenWidth() / 2) - MeasureText(proximaFase, 50) / 2, 350, 50, COR_TITULO);

           //Desenho do Botão START
           Color corTextoBotao;
           if (mouseSobreBotao) {
               corTextoBotao = COR_BOTAO_HOVER;
           } else {
               corTextoBotao = COR_BOTAO_NORMAL;
           }
          
           DrawText(textoBotao, (int)(xBotao - larguraBotao / 2), (int)(yBotao - tamanhoFonteBotao / 2), tamanhoFonteBotao, corTextoBotao);
       }
       EndDrawing();
   }
  
   return acao;
}

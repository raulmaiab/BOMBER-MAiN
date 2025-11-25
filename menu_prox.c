// menu_prox.c

#include "menu_prox.h"
#include "raylib.h"
#include <stdio.h>

// Definições de cores baseadas no tema do menu (ajustadas para Raylib)
#define COR_BOTAO_NORMAL CLITERAL(Color){ 150, 150, 150, 255 }  // Cinza claro para o texto normal
#define COR_BOTAO_HOVER CLITERAL(Color){ 255, 255, 255, 255 }  // Branco para o texto em hover
#define COR_SUCESSO GREEN
#define COR_TITULO RAYWHITE 
#define COR_FUNDO CLITERAL(Color){ 20, 20, 20, 255 } // Quase preto

// --- Lógica do Menu de Próximo Nível ---
AcaoOpcoes ExecutarMenuProximoNivel(InfoNivel info)
{
    AcaoOpcoes acao = OPCOES_ACAO_NENHUMA;
    
    // Posição central do botão START
    int xBotao = GetScreenWidth() / 2;
    int yBotao = (GetScreenHeight() / 2) + 150; 
    
    // Texto do botão
    const char* textoBotao = "INICIAR PRÓXIMO NÍVEL (ENTER)";
    int tamanhoFonteBotao = 40; 
    int larguraBotao = MeasureText(textoBotao, tamanhoFonteBotao);
    
    // Área de colisão do botão (usada para hover e clique)
    Rectangle limitesBotao = { 
        (float)xBotao - (float)larguraBotao / 2, 
        (float)yBotao - (float)tamanhoFonteBotao / 2, 
        (float)larguraBotao, 
        (float)tamanhoFonteBotao 
    };
    
    // Textos informativos
    char nivelConcluido[64];
    sprintf(nivelConcluido, "NÍVEL %d: CONCLUÍDO", info.nivelAtual + 1); 
    
    char proximaFase[64];
    sprintf(proximaFase, "PRÓXIMA FASE: %s", info.proximoMapa);
    
    // CORREÇÃO: !WindowShouldClose() -> WindowShouldClose() == 0
    while (WindowShouldClose() == 0)
    {
        Vector2 pontoMouse = GetMousePosition();
        bool mouseSobreBotao = CheckCollisionPointRec(pontoMouse, limitesBotao);

        // Verifica a entrada do usuário
        if (IsKeyPressed(KEY_ESCAPE)) {
            acao = OPCOES_ACAO_MENU_PRINCIPAL;
            break;
        }

        // --- DETECÇÃO DE BOTÃO (MOUSE OU ENTER) ---
        if (IsKeyPressed(KEY_ENTER) || (mouseSobreBotao && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)))
        {
            // Alterado para OPCOES_ACAO_NENHUMA para indicar a continuação do loop de história
            acao = OPCOES_ACAO_NENHUMA; 
            break;
        }
        // ------------------------------------------

        BeginDrawing();
        {
            ClearBackground(COR_FUNDO);
            
            // Título de Sucesso
            DrawText("VOCÊ VENCEU", 
                     (GetScreenWidth() / 2) - MeasureText("VOCÊ VENCEU", 80) / 2, 
                     100, 
                     80, 
                     COR_SUCESSO);

            // Nível Concluído
            DrawText(nivelConcluido, 
                     (GetScreenWidth() / 2) - MeasureText(nivelConcluido, 35) / 2, 
                     280, 
                     35, 
                     COR_TITULO);

            // Próxima Fase
            DrawText(proximaFase, 
                     (GetScreenWidth() / 2) - MeasureText(proximaFase, 50) / 2, 
                     350, 
                     50, 
                     COR_TITULO);

            // Desenho do Botão START (agora apenas texto com mudança de cor)
            // CORREÇÃO: Uso de if/else para a cor do botão
            Color corTextoBotao;
            if (mouseSobreBotao) {
                corTextoBotao = COR_BOTAO_HOVER;
            } else {
                corTextoBotao = COR_BOTAO_NORMAL;
            }
            
            DrawText(textoBotao,
                     (int)(xBotao - larguraBotao / 2),
                     (int)(yBotao - tamanhoFonteBotao / 2),
                     tamanhoFonteBotao,
                     corTextoBotao);
        }
        EndDrawing();
    }
    
    return acao;
}